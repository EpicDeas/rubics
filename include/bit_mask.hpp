#ifndef BIT_MASK
#define BIT_MASK

#include <array>
#include <type_traits>

#include "opencl_manager.hpp"

using rubics_config = cl_uint4;

// useful bitmasks *******************************
constexpr std::array<cl_uint, 3> edge_pos_mask {
  0b0000000000000000000001111,
  0b0000000000000000011110000,
  0b0000000000000111100000000
};

constexpr std::array<cl_uint, 2> vertex_pos_mask {
  0b0000000000111000000000000,
  0b0000000111000000000000000
};

constexpr std::array<cl_uint, 3> edge_ori_mask {
  0b0000001000000000000000000,
  0b0000010000000000000000000,
  0b0000100000000000000000000
};

constexpr std::array<cl_uint, 2> vertex_ori_mask {
  0b0011000000000000000000000,
  0b1100000000000000000000000
};

constexpr std::array<cl_uint, 5> first_bits {
  0,
  0b1,
  0b11,
  0b111,
  0b1111,
};

// get_edge_pos *******************************
template <int N>
inline cl_uint get_edge_pos (rubics_config* c,
typename std::enable_if<( N < 3 )>::type* = 0) // SFINAE
{
  return (c->s[0] >> N * 4) & first_bits[4];
}

template <int N>
inline cl_uint get_edge_pos (rubics_config* c,
typename std::enable_if<( N >= 3 && N < 6 )>::type* = 0) // SFINAE
{
  return (c->s[1] >> (N - 3) * 4) & first_bits[4];
}

template <int N>
inline cl_uint get_edge_pos (rubics_config* c,
typename std::enable_if<( N >= 6 && N < 9 )>::type* = 0) // SFINAE
{
  return (c->s[2] >> (N - 6) * 4) & first_bits[4];
}

template <int N>
inline cl_uint get_edge_pos (rubics_config* c,
typename std::enable_if<( N >= 9 && N < 12 )>::type* = 0) // SFINAE
{
  return (c->s[3] >> (N - 9) * 4) & first_bits[4];
}

// get_vertex_pos *******************************
template <int N>
inline cl_uint get_vertex_pos (rubics_config* c,
typename std::enable_if<(N < 2)>::type* = 0) // SFINAE
{
  return (c->s[0] >> (12 + N * 3)) & first_bits[3];
}

template <int N>
inline cl_uint get_vertex_pos (rubics_config* c,
typename std::enable_if<(N >= 2 && N < 4)>::type* = 0) // SFINAE
{
  return (c->s[1] >> (12 + (N - 2) * 3)) & first_bits[3];
}

template <int N>
inline cl_uint get_vertex_pos (rubics_config* c,
typename std::enable_if<(N >= 4 && N < 6)>::type* = 0) // SFINAE
{
  return (c->s[2] >> (12 + (N - 4) * 3)) & first_bits[3];
}
template <int N>
inline cl_uint get_vertex_pos (rubics_config* c,
typename std::enable_if<(N >= 6 && N < 8)>::type* = 0) // SFINAE
{
  return (c->s[3] >> (12 + (N - 6) * 3)) & first_bits[3];
}

// get_edge_ori *******************************
template <int N>
inline cl_uint get_edge_ori (rubics_config* c,
typename std::enable_if<(N < 3)>::type* = 0) // SFINAE
{
  return (c->s[0] >> (18 + N)) & first_bits[1];
}

template <int N>
inline cl_uint get_edge_ori (rubics_config* c,
typename std::enable_if<(N >= 3 && N < 6)>::type* = 0) // SFINAE
{
  return (c->s[1] >> (18 + (N - 3))) & first_bits[1];
}

template <int N>
inline cl_uint get_edge_ori (rubics_config* c,
typename std::enable_if<(N >= 6 && N < 9)>::type* = 0) // SFINAE
{
  return (c->s[2] >> (18 + (N - 6))) & first_bits[1];
}

template <int N>
inline cl_uint get_edge_ori (rubics_config* c,
typename std::enable_if<(N >= 9 && N < 12)>::type* = 0) // SFINAE
{
  return (c->s[3] >> (18 + (N - 9))) & first_bits[1];
}

// get_vertex_ori *******************************
template <int N>
inline cl_uint get_vertex_ori (rubics_config* c,
typename std::enable_if<(N < 2)>::type* = 0) // SFINAE
{
  return (c->s[0] >> (21 + N * 2)) & first_bits[2];
}

template <int N>
inline cl_uint get_vertex_ori (rubics_config* c,
typename std::enable_if<(N >= 2 && N < 4)>::type* = 0) // SFINAE
{
  return (c->s[1] >> (21 + (N - 2) * 2)) & first_bits[2];
}

template <int N>
inline cl_uint get_vertex_ori (rubics_config* c,
typename std::enable_if<(N >= 4 && N < 6)>::type* = 0) // SFINAE
{
  return (c->s[2] >> (21 + (N - 4) * 2)) & first_bits[2];
}

template <int N>
inline cl_uint get_vertex_ori (rubics_config* c,
typename std::enable_if<(N >= 6 && N < 8)>::type* = 0) // SFINAE
{
  return (c->s[3] >> (21 + (N - 6) * 2)) & first_bits[2];
}

// set_edge_pos *******************************
template <int N>
inline void set_edge_pos (rubics_config* c, cl_uint a,
typename std::enable_if<(N < 3)>::type* = 0) // SFINAE
{
  c->s[0] = (a << (N * 4)) | (c->s[0] & ~edge_pos_mask[N]);
}

template <int N>
inline void set_edge_pos (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 3 && N < 6)>::type* = 0) // SFINAE
{
  c->s[1] = (a << ((N - 3) * 4)) | (c->s[1] & ~edge_pos_mask[N - 3]);
}

template <int N>
inline void set_edge_pos (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 6 && N < 9)>::type* = 0) // SFINAE
{
  c->s[2] = (a << ((N - 6) * 4)) | (c->s[2] & ~edge_pos_mask[N - 6]);
}

template <int N>
inline void set_edge_pos (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 9 && N < 12)>::type* = 0) // SFINAE
{
  c->s[3] = (a << ((N - 9) * 4)) | (c->s[3] & ~edge_pos_mask[N - 9]);
}

// set_vertex_pos *******************************
template <int N>
inline void set_vertex_pos (rubics_config* c, cl_uint a,
typename std::enable_if<(N < 2)>::type* = 0) // SFINAE
{
  c->s[0] = (a << (12 + N * 3)) | (c->s[0] & ~vertex_pos_mask[N]);
}

template <int N>
inline void set_vertex_pos (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 2 && N < 4)>::type* = 0) // SFINAE
{
  c->s[1] = (a << (12 + (N - 2) * 3)) | (c->s[1] & ~vertex_pos_mask[N - 2]);
}

template <int N>
inline void set_vertex_pos (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 4 && N < 6)>::type* = 0) // SFINAE
{
  c->s[2] = (a << (12 + (N - 4) * 3)) | (c->s[2] & ~vertex_pos_mask[N - 4]);
}

template <int N>
inline void set_vertex_pos (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 6 && N < 8)>::type* = 0) // SFINAE
{
  c->s[3] = (a << (12 + (N - 6) * 3)) | (c->s[3] & ~vertex_pos_mask[N - 6]);
}

// set_edge_ori *******************************
template <int N>
inline void set_edge_ori (rubics_config* c, cl_uint a,
typename std::enable_if<(N < 3)>::type* = 0) // SFINAE
{
  c->s[0] = (a << (18 + N)) | (c->s[0] & ~edge_ori_mask[N]);
}

template <int N>
inline void set_edge_ori (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 3 && N < 6)>::type* = 0) // SFINAE
{
  c->s[1] = (a << (18 + (N - 3))) | (c->s[1] & ~edge_ori_mask[N - 3]);
}

template <int N>
inline void set_edge_ori (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 6 && N < 9)>::type* = 0) // SFINAE
{
  c->s[2] = (a << (18 + (N - 6))) | (c->s[2] & ~edge_ori_mask[N - 6]);
}

template <int N>
inline void set_edge_ori (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 9 && N < 12)>::type* = 0) // SFINAE
{
  c->s[3] = (a << (18 + (N - 9))) | (c->s[3] & ~edge_ori_mask[N - 9]);
}

// set_vertex_ori *******************************
template <int N>
inline void set_vertex_ori (rubics_config* c, cl_uint a,
typename std::enable_if<(N < 2)>::type* = 0) // SFINAE
{
  c->s[0] = (a << (21 + N * 2)) | (c->s[0] & ~vertex_ori_mask[N]);
}

template <int N>
inline void set_vertex_ori (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 2 && N < 4)>::type* = 0) // SFINAE
{
  c->s[1] = (a << (21 + (N - 2) * 2)) | (c->s[1] & ~vertex_ori_mask[N - 2]);
}

template <int N>
inline void set_vertex_ori (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 4 && N < 6)>::type* = 0) // SFINAE
{
  c->s[2] = (a << (21 + (N - 4) * 2)) | (c->s[2] & ~vertex_ori_mask[N - 4]);
}
template <int N>
inline void set_vertex_ori (rubics_config* c, cl_uint a,
typename std::enable_if<(N >= 6 && N < 8)>::type* = 0) // SFINAE
{
  c->s[3] = (a << (21 + (N - 6) * 2)) | (c->s[3] & ~vertex_ori_mask[N - 6]);
}

#endif