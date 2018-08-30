#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <random>
#include <assert.h>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include "configuration.hpp"
#include "bit_mask.hpp"
#include "opencl_manager.hpp"

constexpr size_t WORKGROUP_SIZE = 1024;
constexpr size_t LEVEL_COUNT = 4;

constexpr std::array<size_t, 5> BUFFER_SIZES {
  WORKGROUP_SIZE,
  WORKGROUP_SIZE * 18,
  WORKGROUP_SIZE * 18 * 18,
  WORKGROUP_SIZE * 18 * 18 * 18,
  WORKGROUP_SIZE * 18 * 18 * 18 * 18
};

constexpr size_t MAX_BUFFER_SIZE = BUFFER_SIZES[LEVEL_COUNT - 1];

std::vector<rubics_config> init_pool_vec(rubics_config c)
{
  // populate the main pool with some initial configurations
  // this is done on the host since it is easier 
  // and the task is small

  std::vector<rubics_config> vec(WORKGROUP_SIZE);
  vec[0] = c;

  // init randomness
  std::mt19937 rng;
  rng.seed(0); // for now the seed is deterministic
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

OpenCLManager::OpenCLManager(const rubics_config& c) :
  reduce_functor(cl::Kernel())
{
  init_config = c;

  context = cl::Context(CL_DEVICE_TYPE_DEFAULT);
  auto file_str = read_file("src/kernels/main_kernels.cl");

  try
  {
    program = cl::Program(context, file_str);
    program.build("-I src/kernels");
    queue = cl::CommandQueue(context);

    // initialize kernel functors
    for (int i = 0; i < 6; ++i)
    {
      move_functors.emplace_back(program, move_kernel_names[i]);
      std::cout << "Initialized kernel " << move_kernel_names[i] << ".\n";
    }

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
    for (size_t level = 0; level < LEVEL_COUNT - 1; ++level)
    {
      for (size_t in_offset = 0;
                  in_offset < BUFFER_SIZES[level];
                  in_offset += WORKGROUP_SIZE)
      {
        for (int functor_index = 0; functor_index < 6; ++functor_index)
        {
          for (int count = 1; count <= 3; ++count)
          {
            auto out_offset = in_offset * 18 + (functor_index * 3 + count) * WORKGROUP_SIZE;
            move_functors[functor_index](
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
    queue.finish();
  }
  catch(cl::Error error)
  {
    std::cerr << "what(): " << error.what() << std::endl;
    exit(1);
  }
}

void OpenCLManager::reduce()
{
  try
  {
    for (size_t level = LEVEL_COUNT - 1; level > 0; --level)
    {
      for (size_t out_offset_rough = 0;
                  out_offset_rough < BUFFER_SIZES[level - 1];
                  out_offset_rough += WORKGROUP_SIZE)
      {
        for (int i = 0; i < 18; ++i)
        {
          size_t in_offset = 18 * out_offset_rough + i * WORKGROUP_SIZE;
          size_t out_offset = out_offset_rough + i * 50; // write with stride of 50 words
          reduce_functor(
            cl::EnqueueArgs(
              queue,
              cl::NDRange(WORKGROUP_SIZE),   // global dimensions
              cl::NDRange(WORKGROUP_SIZE)    // local dimensions
            ),
            buffer_pool[level],              // in buffer
            buffer_pool[level - 1],          // out buffer
            in_offset,                       // in offset
            WORKGROUP_SIZE,                  // in size
            out_offset,                      // out offset
            18                               // out size
          );
        }
      }
    }
  }
  catch(cl::Error error)
  {
    std::cerr << "what(): " << error.what() << std::endl;
    exit(1);
  }
}

bool OpenCLManager::move_correctness_check()
{
  try
  {
    // initialize output buffers
    std::vector<cl::Buffer> buffer_vec;
    for (int i = 0; i < 6; i++) 
      buffer_vec.emplace_back(context, CL_MEM_WRITE_ONLY, sizeof(rubics_config));

    // initialize input buffer
    std::vector<rubics_config> host_vec(1, SOLVED_CONFIG);
    cl::Buffer in_solved(context, std::begin(host_vec), std::end(host_vec), true); 

    // enqueue the kernels
    for (int functor_index = 0; functor_index < 6; ++functor_index)
    {
      move_functors[functor_index](
        cl::EnqueueArgs(
          queue,
          cl::NDRange(1)             // global dimensions
        ),
        in_solved,                   // in buffer
        buffer_vec[functor_index],   // out buffer
        1,                           // count
        0,                           // in offset
        0                            // out offset
      );
    }
    queue.finish();    
    
    // read the results and compare to the host
    for (int functor_index = 0; functor_index < 6; ++functor_index)
    {
      cl::copy(queue, buffer_vec[functor_index], std::begin(host_vec), std::end(host_vec)); // blocking copy
      rubics_config c = SOLVED_CONFIG;
      host_move_functors[functor_index](&c, 1);
      if (!(c == host_vec[0]))
      {
        std::cout << "right:\n" << c << "\nwrong:\n" << host_vec[0] << "\n";
      }
    }
  }
  catch(cl::Error error)
  {
    std::cerr << "what(): " << error.what() << std::endl;
  }
  return true;
}
