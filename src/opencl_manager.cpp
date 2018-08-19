#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <random>
#include <functional>
#include <assert.h>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include "configuration.hpp"
#include "bit_mask.hpp"
#include "opencl_manager.hpp"

constexpr auto WORKGROUP_SIZE = 1024;

const std::array<std::string, 6> move_kernel_names {
  "move_F", "move_B", "move_L", "move_R", "move_U", "move_D"
};

const std::array<std::function<void(rubics_config*, size_t)>, 6> move_functors {
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
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist_move(0,5);
    std::uniform_int_distribution<std::mt19937::result_type> dist_count(1,3);
  // obtain random number by dist(rng)

  for (int i = 1; i < WORKGROUP_SIZE; ++i)
  {
    auto move_type = dist_move(rng);
    auto move_count = dist_count(rng);
    move_functors[move_type](&c, move_count);
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
  context = cl::Context(CL_DEVICE_TYPE_DEFAULT);
  auto file_str = read_file("src/kernels/moves.cl");

  try
  {
    program = cl::Program(context, file_str);
    program.build("-I src/kernels");
    queue = cl::CommandQueue(context);

    auto f = cl::make_kernel<
      cl::Buffer,
      cl::Buffer,
      cl::Buffer,
      const int>(program, "move_F");

    for (int i = 0; i < 6; ++i)
      move_functor.emplace_back(program, move_kernel_names[i]);
    
    // generate initial configurations
    auto pool_vec = init_pool_vec(c);

    // initialize main pool
    main_pool = std::make_unique<cl::Buffer>(
      context,
      std::begin(pool_vec),
      std::end(pool_vec),
      false
    );
  }
  catch(cl::Error error)
  {
    std::cerr << "what(): " << error.what() << std::endl;
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    cl::Device device = devices[0];
    std::cerr << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
  }
}

void OpenCLManager::compute_rounds(int count)
{
  try
  {
    queue.finish();
  }
  catch(cl::Error error)
  {
    std::cerr << "what(): " << error.what() << std::endl;
  }
}
