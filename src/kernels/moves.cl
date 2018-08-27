#include "move.c"
#include "close_configs.h"
#include "evaluate.c"

#define MOVE(X, EA, EB, EC, ED, VA, VB, VC, VD, edge_ori, vertex_ori)\
__kernel                                                             \
void move_##X(                                                       \
  __global const rubics_config* restrict in,                         \
  __global       rubics_config* restrict out,                        \
  int count,                                                         \
  const int in_offset,                                               \
  const int out_offset)                                              \
{                                                                    \
  int gid = get_global_id(0);                                        \
                                                                     \
  rubics_config config = in[gid + in_offset];                        \
                                                                     \
  for (; count; --count)                                             \
    move(EA, EB, EC, ED,                                             \
         VA, VB, VC, VD,                                             \
         &config, edge_ori, vertex_ori);                             \
                                                                     \
  evaluate(&config);                                                 \
  out[gid + out_offset] = config;                                    \
}

MOVE(F, 2, 5, 8, 4,
        2, 5, 4, 3, true, 1)

MOVE(B, 0, 7, 10, 6,
        0, 7,  6, 1, true, 1)

MOVE(L, 3, 4, 11, 7,
        0, 3,  4, 7, false, 2)

MOVE(R, 1, 6, 9, 5,
        1, 6, 5, 2, false, 1)

MOVE(U, 0, 1, 2, 3,
        0, 1, 2, 3, false, 0)

MOVE(D, 8, 9, 10, 11,
        4, 5, 6, 7, false, 0)

