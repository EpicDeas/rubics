#include "bit_mask.h"

typedef uint4 rubics_config;

inline uint flip_edge_ori (uint a)
{
  return a ^ 1;
}

inline uint add_vertex_ori (uint a)
{
  return (a + 1) % 3;
}

inline uint drop_vertex_ori (uint a)
{
  return (a + 2) % 3;
}

#define GET_EDGE_POS_P(N, c, P)\
  (c->P >> (N * 4)) & 15

uint get_edge_pos(int N, rubics_config* c)
{
  if (N < 3)
    return GET_EDGE_POS_P(N,     c, x);
  else if (N < 6)
    return GET_EDGE_POS_P(N - 3, c, y);
  else if (N < 9)
    return GET_EDGE_POS_P(N - 6, c, z);
  else
    return GET_EDGE_POS_P(N - 9, c, w);
}

#define GET_VERTEX_POS_P(N, c, P)\
  (c->P >> (12 + (N * 3))) & 7

uint get_vertex_pos(int N, rubics_config* c)
{
  if (N < 2)
    return GET_VERTEX_POS_P(N,     c, x);
  else if (N < 4)
    return GET_VERTEX_POS_P(N - 2, c, y);
  else if (N < 6)
    return GET_VERTEX_POS_P(N - 4, c, z);
  else
    return GET_VERTEX_POS_P(N - 6, c, w);
}

#define GET_EDGE_ORI_P(N, c, P)\
  (c->P >> (18 + N)) & 1

uint get_edge_ori(int N, rubics_config* c)
{
  if (N < 3)
    return GET_EDGE_ORI_P(N,     c, x);
  else if (N < 6)
    return GET_EDGE_ORI_P(N - 3, c, y);
  else if (N < 9)
    return GET_EDGE_ORI_P(N - 6, c, z);
  else
    return GET_EDGE_ORI_P(N - 9, c, w);
}

#define GET_VERTEX_ORI_P(N, c, P)\
  (c->P >> (21 + (N * 2))) & 3

uint get_vertex_ori(int N, rubics_config* c)
{
  if (N < 2)
    return GET_VERTEX_ORI_P(N,     c, x);
  else if (N < 4)
    return GET_VERTEX_ORI_P(N - 2, c, y);
  else if (N < 6)
    return GET_VERTEX_ORI_P(N - 4, c, z);
  else
    return GET_VERTEX_ORI_P(N - 6, c, w);
}

#define SET_EDGE_POS_P(N, c, e, P)\
  c->P = (e << (N * 4)) | (c->P & ~EDGE_POS(N))

void set_edge_pos(int N, rubics_config* c, uint e)
{
  if (N < 3)
    SET_EDGE_POS_P(N,     c, e, x);
  else if (N < 6)
    SET_EDGE_POS_P(N - 3, c, e, y);
  else if (N < 9)
    SET_EDGE_POS_P(N - 6, c, e, z);
  else
    SET_EDGE_POS_P(N - 9, c, e, w);
}

#define SET_VERTEX_POS_P(N, c, v, P)\
  c->P = (v << (12 + (N * 3))) | (c->P & ~VERTEX_POS(N))

void set_vertex_pos(int N, rubics_config* c, uint v)
{
  if (N < 2)
    SET_VERTEX_POS_P(N,     c, v, x);
  else if (N < 4)
    SET_VERTEX_POS_P(N - 2, c, v, y);
  else if (N < 6)
    SET_VERTEX_POS_P(N - 4, c, v, z);
  else
    SET_VERTEX_POS_P(N - 6, c, v, w);
}

#define SET_EDGE_ORI_P(N, c, e, P)\
  c->P = (e << (18 + N)) | (c->P & ~EDGE_ORI(N))

void set_edge_ori(int N, rubics_config* c, uint e)
{
  if (N < 3)
    SET_EDGE_ORI_P(N,     c, e, x);
  else if (N < 6)
    SET_EDGE_ORI_P(N - 3, c, e, y);
  else if (N < 9)
    SET_EDGE_ORI_P(N - 6, c, e, z);
  else
    SET_EDGE_ORI_P(N - 9, c, e, w);
}

#define SET_VERTEX_ORI_P(N, c, v, P)\
  c->P = (v << (21 + (N * 2))) | (c->P & ~VERTEX_ORI(N))

void set_vertex_ori(int N, rubics_config* c, uint v)
{
  if (N < 2)
    SET_VERTEX_ORI_P(N,     c, v, x);
  else if (N < 4)
    SET_VERTEX_ORI_P(N - 2, c, v, y);
  else if (N < 6)
    SET_VERTEX_ORI_P(N - 4, c, v, z);
  else
    SET_VERTEX_ORI_P(N - 6, c, v, w);
}

// this could be a macro for better performance?
void move(
  int EA, int EB, int EC, int ED,
  int VA, int VB, int VC, int VD, 
  rubics_config* c,
  bool flip_edges,
  int vertex_ori_pattern)
{
  uint e1 = get_edge_pos(EA, c);
  uint e2 = get_edge_pos(EB, c);
  uint e3 = get_edge_pos(EC, c);
  uint e4 = get_edge_pos(ED, c);
  set_edge_pos(EA, c, e4);
  set_edge_pos(EB, c, e1);
  set_edge_pos(EC, c, e2);
  set_edge_pos(ED, c, e3);
  uint v1 = get_vertex_pos(VA, c);
  uint v2 = get_vertex_pos(VB, c);
  uint v3 = get_vertex_pos(VC, c);
  uint v4 = get_vertex_pos(VD, c);
  set_vertex_pos(VA, c, v4);
  set_vertex_pos(VB, c, v1);
  set_vertex_pos(VC, c, v2);
  set_vertex_pos(VD, c, v3);
  if (flip_edges)
  {
    uint eo1 = get_edge_ori(EA, c);
    uint eo2 = get_edge_ori(EB, c);
    uint eo3 = get_edge_ori(EC, c);
    uint eo4 = get_edge_ori(ED, c);
    set_edge_ori(EB, c, flip_edge_ori(eo1));
    set_edge_ori(EC, c, flip_edge_ori(eo2));
    set_edge_ori(ED, c, flip_edge_ori(eo3));
    set_edge_ori(EA, c, flip_edge_ori(eo4));
  }
  if (vertex_ori_pattern > 0) 
  {
    uint vo1 = get_vertex_ori(VA, c);
    uint vo2 = get_vertex_ori(VB, c);
    uint vo3 = get_vertex_ori(VC, c);
    uint vo4 = get_vertex_ori(VD, c);
    if (vertex_ori_pattern == 1)
    {
      set_vertex_ori(VB, c, add_vertex_ori(vo1));
      set_vertex_ori(VC, c, drop_vertex_ori(vo2));
      set_vertex_ori(VD, c, add_vertex_ori(vo3));
      set_vertex_ori(VA, c, drop_vertex_ori(vo4));
    } 
    else
    {
      set_vertex_ori(VB, c, drop_vertex_ori(vo1));
      set_vertex_ori(VC, c, add_vertex_ori(vo2));
      set_vertex_ori(VD, c, drop_vertex_ori(vo3));
      set_vertex_ori(VA, c, add_vertex_ori(vo4));
    }
  }
}