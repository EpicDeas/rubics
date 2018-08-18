#ifndef CONFIGURATION
#define CONFIGURATION

#include <array>
#include <string>
#include <fstream>

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

// representation of solved cube

constexpr rubics_config SOLVED_CONFIG {
  0b00'00'000'001'000'0010'0001'0000,
  0b00'00'000'011'010'0101'0100'0011,
  0b00'00'000'101'100'1000'0111'0110,
  0b00'00'000'111'110'1011'1010'1001
};

rubics_config parse_rubics(std::string line);

void move_F (rubics_config* c, size_t count);
void move_B (rubics_config* c, size_t count);
void move_L (rubics_config* c, size_t count);
void move_R (rubics_config* c, size_t count);
void move_U (rubics_config* c, size_t count);
void move_D (rubics_config* c, size_t count);

#endif