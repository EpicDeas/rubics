#include <iostream>
#include <fstream>
#include <vector>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include "opencl_manager.hpp"

OpenCLManager::OpenCLManager()
{
  context = cl::Context(CL_DEVICE_TYPE_DEFAULT);
  std::ifstream file("src/kernels/moves.cl");
  std::string file_str(
    (std::istreambuf_iterator<char>(file)),
    (std::istreambuf_iterator<char>()));

  try
  {
    program = cl::Program(context, file_str);
    program.build("-I src/kernels");
    queue = cl::CommandQueue(context);
  }
  catch(cl::Error error)
  {
    std::cout << "what(): " << error.what() << std::endl;
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    cl::Device device = devices[0];
    std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
  }
}