#include "move.c"

__kernel
void move_F(
  __global const uint4* restrict in,
  __global       uint4* restrict out,
  const int N)
{
  int gid = get_global_id(0);
  if (gid < N)
  {
    rubics_config config = in[gid];

    move(2, 5, 8, 4,
         2, 5, 4, 3,
         &config, true, 1);

    out[gid] = config;
  }
}

__kernel
void move_B(
  __global const uint4* restrict in,
  __global       uint4* restrict out,
  const int N)
{
  int gid = get_global_id(0);
  if (gid < N)
  {
    rubics_config config = in[gid];
    move(0, 7, 10, 6,
         0, 7,  6, 1,
         &config, true, 1);
         
    out[gid] = config;
  }
}

__kernel
void move_L(
  __global const uint4* restrict in,
  __global       uint4* restrict out,
  const int N)
{
  int gid = get_global_id(0);
  if (gid < N)
  {
    rubics_config config = in[gid];

    move(3, 4, 11, 7,
         0, 3,  4, 7,
         &config, false, 2);
         
    out[gid] = config;
  }
}

__kernel
void move_R(
  __global const uint4* restrict in,
  __global       uint4* restrict out,
  const int N)
{
  int gid = get_global_id(0);
  if (gid < N)
  {
    rubics_config config = in[gid];

    move(1, 6, 9, 5,
         1, 6, 5, 2,
         &config, false, 1);
         
    out[gid] = config;
  }
}

__kernel
void move_U(
  __global const uint4* restrict in,
  __global       uint4* restrict out,
  const int N)
{
  int gid = get_global_id(0);
  if (gid < N)
  {
    rubics_config config = in[gid];

    move(0, 1, 2, 3,
         0, 1, 2, 3,
         &config, false, 0);
         
    out[gid] = config;
  }
}

__kernel
void move_D(
  __global const uint4* restrict in,
  __global       uint4* restrict out,
  const int N)
{
  int gid = get_global_id(0);
  if (gid < N)
  {
    rubics_config config = in[gid];

    move(8, 9, 10, 11,
         4, 5,  6, 7,
         &config, false, 0);
         
    out[gid] = config;
  }
}
