#ifndef CONFIGURATION
#define CONFIGURATION

#include <array>
#include <string>
#include <fstream>
#include <iostream>
#include <functional>

#include "opencl_manager.hpp"
#include "bit_mask.hpp"

// logging mechanism fully implemented in the header
class log
{
private:
  std::ofstream log_file;
public:
  log() : log_file("log.txt") {}
  static inline void write(const std::string& str)
  {
    static log l;
    l.log_file << str;
  }
};

inline bool operator==(const rubics_config& left, const rubics_config& right)
{
  for (int i = 0; i < 4; ++i)
  {
    if (left.s[i] != right.s[i])
      return false;
  }
  return true;
}

inline std::ostream& operator<<(std::ostream& s, const rubics_config& c)
{
  return s << c.s[0] << '\n' << c.s[1] << '\n'
           << c.s[2] << '\n' << c.s[3] << '\n';
}

// representation of solved cube

constexpr rubics_config SOLVED_CONFIG {
  0b00'00'000'001'000'0010'0001'0000,
  0b00'00'000'011'010'0101'0100'0011,
  0b00'00'000'101'100'1000'0111'0110,
  0b00'00'000'111'110'1011'1010'1001
};

constexpr rubics_config EMPTY_CONFIG {
  0b00'00'000'000'000'0000'0000'0000,
  0b00'00'000'000'000'0000'0000'0000,
  0b00'00'000'000'000'0000'0000'0000,
  0b00'00'000'000'000'0000'0000'0000
};

int evaluate(rubics_config* c);

rubics_config parse_rubics(std::string line);

void move_F (rubics_config* c, size_t count);
void move_B (rubics_config* c, size_t count);
void move_L (rubics_config* c, size_t count);
void move_R (rubics_config* c, size_t count);
void move_U (rubics_config* c, size_t count);
void move_D (rubics_config* c, size_t count);

const std::array<std::string, 6>
move_kernel_names {
  "move_F", "move_B", "move_L", "move_R", "move_U", "move_D"
};

const std::array<std::function<void(rubics_config*, size_t)>, 6>
host_move_functors {
  move_F, move_B, move_L, move_R, move_U, move_D
};

inline cl_uint flip_edge_ori (cl_uint a)
{
  return a ^ 1;
}

inline cl_uint add_vertex_ori (cl_uint a)
{
  return (a + 1) % 3;
}

inline cl_uint drop_vertex_ori (cl_uint a)
{
  return (a + 2) % 3;
}

#endif