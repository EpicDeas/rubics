#include <iostream>
#include <fstream>

#include "generate_kernels.hpp"
#include "configuration.hpp"

void print_src(rubics_config c, std::ofstream& ofs)
{
  ofs << "(rubics_config)(" << c.s[0] << ","
                            << c.s[1] << ","
                            << c.s[2] << ","
                            << c.s[3] << "),\n  ";
}

void generate_close_configs()
{
  std::ofstream ofs("src/kernels/close_configs.h");
  ofs << "#define CLOSE_CONFIGS_COUNT 19\n";
  ofs << "\n__constant const rubics_config CLOSE_CONFIGS[] = {\n  ";
  for (auto f : host_move_functors)
  {
    for (int i = 1; i <= 3; i++)
    {
      rubics_config c = SOLVED_CONFIG;
      f(&c, i);
      print_src(c, ofs);
    }
  }
  print_src(SOLVED_CONFIG, ofs);
  ofs.seekp((int)ofs.tellp() - 4);
  ofs << "\n};\n";
}