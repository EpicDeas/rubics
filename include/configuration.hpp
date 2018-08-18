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

// representation of solved cube
constexpr rubics_config SOLVED_CONFIG {
  0b0000'0001'0010'000'001'000'00'00,
  0b0011'0100'0101'010'011'000'00'00,
  0b0110'0111'1000'100'101'000'00'00,
  0b1001'1010'1011'110'111'000'00'00
};

rubics_config parse_rubics(std::string line);

void move_F (rubics_config* c, size_t count);
void move_B (rubics_config* c, size_t count);
void move_L (rubics_config* c, size_t count);
void move_R (rubics_config* c, size_t count);
void move_U (rubics_config* c, size_t count);
void move_D (rubics_config* c, size_t count);

#endif