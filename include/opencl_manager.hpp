#ifndef OPENCL_MANAGER
#define OPENCL_MANAGER

#include <vector>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include "bit_mask.hpp"

//    My GPU spec:
//              Max. Compute Units: 2
//              Local Memory Size: 48 KB
//              Global Memory Size: 1998 MB
//              Max Alloc Size: 499 MB
//              Max Work-group Total Size: 1024
//              Max Work-group Dims: (1024 1024 64)

class OpenCLManager
{
  using move_functor_t = cl::make_kernel<cl::Buffer, cl::Buffer, int, const int, const int>;
private:
  cl::Context      context;
  cl::CommandQueue queue;
  cl::Program      program;

  std::vector<cl::Buffer> buffer_pool;
  std::vector<move_functor_t> move_functor;

  rubics_config init_config;

public:
  OpenCLManager(const rubics_config& c);
  void compute_round();
  bool move_correctness_check();
};

#endif