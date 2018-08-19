#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <random>
#include <functional>
#include <assert.h>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include "configuration.hpp"
#include "bit_mask.hpp"
#include "opencl_manager.hpp"

constexpr auto WORKGROUP_SIZE = 1024;

constexpr auto LEVEL_COUNT = 4;

constexpr std::array<size_t, 5> BUFFER_SIZES {
  WORKGROUP_SIZE,
  WORKGROUP_SIZE * 18,
  WORKGROUP_SIZE * 18 * 18,
  WORKGROUP_SIZE * 18 * 18 * 18,
  WORKGROUP_SIZE * 18 * 18 * 18 * 18
};

constexpr size_t MAIN_BUFFER_SIZE = 111151 * WORKGROUP_SIZE;

const std::array<std::string, 6>
move_kernel_names {
  "move_F", "move_B", "move_L", "move_R", "move_U", "move_D"
};

const std::array<std::function<void(rubics_config*, size_t)>, 6>
host_move_functors {
  move_F, move_B, move_L, move_R, move_U, move_D
};

std::vector<rubics_config> init_pool_vec(rubics_config c)
{
  // populate the main pool with some initial configurations
  // this is done on the host since it is easier 
  // and the task is small

  std::vector<rubics_config> vec(WORKGROUP_SIZE);
  vec[0] = c;

  // init randomness
    std::mt19937 rng;
    rng.seed(0);
    std::uniform_int_distribution<std::mt19937::result_type> dist_move(0,5);
    std::uniform_int_distribution<std::mt19937::result_type> dist_count(1,3);
  // obtain random number by dist(rng)

  for (int i = 1; i < WORKGROUP_SIZE; ++i)
  {
    auto move_type = dist_move(rng);
    auto move_count = dist_count(rng);
    host_move_functors[move_type](&c, move_count);
    vec[i] = c;
  }

  return vec;
}

std::string read_file(const std::string& filename)
{
  std::ifstream file(filename);
  return {
    (std::istreambuf_iterator<char>(file)),
    (std::istreambuf_iterator<char>())
  };
}

OpenCLManager::OpenCLManager(const rubics_config& c)
{
  init_config = c;

  context = cl::Context(CL_DEVICE_TYPE_DEFAULT);
  auto file_str = read_file("src/kernels/moves.cl");

  try
  {
    program = cl::Program(context, file_str);
    program.build("-I src/kernels");
    queue = cl::CommandQueue(context);

    // initialize kernel functors
    for (int i = 0; i < 6; ++i)
      move_functor.emplace_back(program, move_kernel_names[i]);

    // initialize all buffers
    for (int i = 0; i < LEVEL_COUNT; ++i)
    {
      buffer_pool.emplace_back(
        context,
        CL_MEM_READ_WRITE,
        sizeof(rubics_config) * BUFFER_SIZES[i]
      );
    }

    // generate initial configurations
    auto init_vec = init_pool_vec(init_config);

    // blocking copy
    cl::copy(
      queue,
      std::begin(init_vec),
      std::end(init_vec),
      buffer_pool[0]
    );
  }
  catch(cl::Error error)
  {
    std::cerr << "what(): " << error.what() << std::endl;
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
    std::cerr << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;
  }
}

void OpenCLManager::compute_round()
{
  try
  {
    for (int level = 0; level < LEVEL_COUNT - 1; ++level)
    {
      for (int in_offset = 0;
               in_offset < BUFFER_SIZES[level];
               in_offset += WORKGROUP_SIZE)
      {
        log::write("Executing level " + std::to_string(level) + '\n');
        for (int functor_index = 0; functor_index < 6; ++functor_index)
        {
          for (int count = 0; count < 3; ++count)
          {
            auto out_offset = in_offset * 18 + (functor_index * 3 + count) * WORKGROUP_SIZE;
            move_functor[functor_index](
              cl::EnqueueArgs(
                queue,
                cl::NDRange(WORKGROUP_SIZE),        // global dimensions
                cl::NDRange(WORKGROUP_SIZE)         // local dimensions
              ),
              buffer_pool[level],                   // in buffer
              buffer_pool[level + 1],               // out buffer
              count,
              in_offset,
              out_offset
            );
          }
        }
      }
    }
            // DEBUG
    std::vector<rubics_config> host;
    for (int level = 0; level < LEVEL_COUNT; ++level)
    {
      host.resize(BUFFER_SIZES[level]);
      // blocking copy
      cl::copy(
        queue,
        buffer_pool[level],
        std::begin(host),
        std::end(host)
      );
      // sanity check
      for (int i = 0; i < BUFFER_SIZES[level]; ++i)
        if (EMPTY_CONFIG == host[i])
          std::cout << i;
    }
    queue.finish();
  }
  catch(cl::Error error)
  {
    std::cerr << "what(): " << error.what() << std::endl;
  }
}
