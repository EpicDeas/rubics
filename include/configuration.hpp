#ifndef CONFIGURATION
#define CONFIGURATION

#include <array>
#include <string>

#include "opencl_manager.hpp"
#include "bit_mask.hpp"

constexpr rubics_config SOLVED_CONFIG {
  0b0000000100100000010000000,
  0b0100010101100100110000000,
  0b0111100010011001010000000,
  0b1010101111001101110000000
};

rubics_config parse_rubics(std::string line);

void move_F (rubics_config* c, size_t count);
void move_B (rubics_config* c, size_t count);
void move_L (rubics_config* c, size_t count);
void move_R (rubics_config* c, size_t count);
void move_U (rubics_config* c, size_t count);
void move_D (rubics_config* c, size_t count);

#endif