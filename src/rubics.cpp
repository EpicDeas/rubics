#include <iostream>
#include <algorithm>
#include <fstream>

#include "rubics.hpp"

// Rubics cube has 8 vertices and 12 edges.
// That is 20 positions to describe in total.
// A piece can be identified by its category (vertex, edge)
// and its number (at most 12 possibilities -- 4 bits).
// We need 20 * 4 = 80 bits ~ 3 * 32bit numbers.

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Specify filename with scramble as a cmd parameter.\n";
    return 1;
  }

  std::string filename(argv[1]), line;
  std::ifstream ifs(filename);
  std::getline(ifs, line);

  rubics_config config = parse_rubics(line);

  // initialize OpenCL
  OpenCLManager cl_mng;
  return 0;
}