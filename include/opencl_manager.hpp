#ifndef OPENCL_MANAGER
#define OPENCL_MANAGER

#include <CL/cl.hpp>

class OpenCLManager
{
private:
  cl::Context context;
  cl::CommandQueue queue;
  cl::Program program;
public:
  OpenCLManager();
};

#endif