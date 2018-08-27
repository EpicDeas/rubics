#include <iostream>
#include <algorithm>
#include <fstream>
#include <istream>

#include "opencl_manager.hpp"
#include "configuration.hpp"
#include "generate_kernels.hpp"

// Rubics cube has 8 vertices and 12 edges.
// That is 20 positions to describe in total.
// A piece can be identified by its category (vertex, edge)
// and its number (at most 12 possibilities -- 4 bits).
// We need 20 * 4 = 80 bits ~ 3 * 32bit numbers.

rubics_config parse_param(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Specify filename with scramble as a cmd parameter.\n";
    exit(1);
  }
          
  std::string filename(argv[1]), line;
  std::ifstream ifs(filename);
  std::getline(ifs, line);

  return parse_rubics(line);
}

int main(int argc, char** argv)
{
  rubics_config config = parse_param(argc, argv);

  generate_close_configs();

  // initialize OpenCL
  OpenCLManager mng(config);

  if (mng.move_correctness_check())
    std::cout << "Correctness check passed.\n";
  else
  {
    std::cout << "Correctness check failed!\n";
    exit(1);
  }

  // run
  //mng.compute_round();
  return 0;
}