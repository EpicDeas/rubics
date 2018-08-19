#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>

#include "configuration.hpp"

void cmd_switch(const std::string& cmd, rubics_config* c, bool reverse = false)
{
  // number of turns
  auto count = reverse ? 3 : 1;
  if (cmd.size() > 1)
  {
    if (cmd[1] == '2')
      count = 2;
    else
      count = reverse ? 1 : 3;
  }
  // which command
  switch (cmd[0])
  {
    case 'F': move_F(c, count); break;
    case 'B': move_B(c, count); break;
    case 'L': move_L(c, count); break;
    case 'R': move_R(c, count); break;
    case 'U': move_U(c, count); break;
    case 'D': move_D(c, count); break;
  }
}

rubics_config parse_rubics(std::string line)
{
  std::vector<std::string> tokens;

  // split into tokens
  std::istringstream iss{ line };
  std::copy(
    std::istream_iterator<std::string>(iss),
    std::istream_iterator<std::string>(),
    std::back_inserter(tokens));
  
  // start with solved rubics cube
  auto config{ SOLVED_CONFIG };

  // apply commands one by one
  for (const auto& cmd : tokens)
    cmd_switch(cmd, &config);

  // sanity check
  //log::write("starting sanity check\n");
  //for(auto it = tokens.rbegin(); it != tokens.rend(); it++)
  //  cmd_switch(*it, &config, true);
  //if (config == SOLVED_CONFIG)
  //  log::write("Sanity check passed!\n");
  //else 
  //  log::write("Sanity check failed!\n");

  return config;
}

int evaluate(rubics_config* c)
{
  int diff = 0;
  for (int i = 0; i < 4; ++i)
  {
    auto temp = c->s[i] ^ SOLVED_CONFIG.s[i];
    for (int j = 0; j < 3; ++j)
    {
      diff += temp & edge_pos_mask[j] ? 1 : 0;
      diff += temp & edge_ori_mask[j] ? 1 : 0;
    }
    for (int j = 0; j < 2; ++j)
    {
      diff += temp & vertex_pos_mask[j] ? 1 : 0;
      diff += temp & vertex_ori_mask[j] ? 1 : 0;
    }
  }
  return diff;
}

// move implementations
template <int EA, int EB, int EC, int ED,
          int VA, int VB, int VC, int VD>
void move_pos (rubics_config* c, bool flip_edges, int vertex_ori_pattern)
{
  // edge positions
  auto e1 = get_edge_pos<EA>(c); 
  auto e2 = get_edge_pos<EB>(c);
  auto e3 = get_edge_pos<EC>(c);
  auto e4 = get_edge_pos<ED>(c);
  set_edge_pos<EA>(c, e4);
  set_edge_pos<EB>(c, e1);
  set_edge_pos<EC>(c, e2);
  set_edge_pos<ED>(c, e3);
  // vertex positions
  auto v1 = get_vertex_pos<VA>(c);
  auto v2 = get_vertex_pos<VB>(c);
  auto v3 = get_vertex_pos<VC>(c);
  auto v4 = get_vertex_pos<VD>(c);
  set_vertex_pos<VA>(c, v4);
  set_vertex_pos<VB>(c, v1);
  set_vertex_pos<VC>(c, v2);
  set_vertex_pos<VD>(c, v3);

  if (flip_edges)
  {
    auto eo1 = get_edge_ori<EA>(c);
    auto eo2 = get_edge_ori<EB>(c);
    auto eo3 = get_edge_ori<EC>(c);
    auto eo4 = get_edge_ori<ED>(c);
    set_edge_ori<EB>(c, flip_edge_ori(eo1));
    set_edge_ori<EC>(c, flip_edge_ori(eo2));
    set_edge_ori<ED>(c, flip_edge_ori(eo3));
    set_edge_ori<EA>(c, flip_edge_ori(eo4));
  }

  if (vertex_ori_pattern > 0) 
  {
    auto vo1 = get_vertex_ori<VA>(c);
    auto vo2 = get_vertex_ori<VB>(c);
    auto vo3 = get_vertex_ori<VC>(c);
    auto vo4 = get_vertex_ori<VD>(c);
    if (vertex_ori_pattern == 1)
    {
      set_vertex_ori<VB>(c, add_vertex_ori(vo1));
      set_vertex_ori<VC>(c, drop_vertex_ori(vo2));
      set_vertex_ori<VD>(c, add_vertex_ori(vo3));
      set_vertex_ori<VA>(c, drop_vertex_ori(vo4));
    } 
    else
    {
      set_vertex_ori<VB>(c, drop_vertex_ori(vo1));
      set_vertex_ori<VC>(c, add_vertex_ori(vo2));
      set_vertex_ori<VD>(c, drop_vertex_ori(vo3));
      set_vertex_ori<VA>(c, add_vertex_ori(vo4));
    }
  }
}

void move_F (rubics_config* c, size_t count)
{
  for (; count; --count)
    move_pos<2, 5, 8, 4,
             2, 5, 4, 3>(c, true, 1);
}

void move_B (rubics_config* c, size_t count)
{
  for (; count; --count)
    move_pos<0, 7, 10, 6,
             0, 7,  6, 1>(c, true, 1);
}

void move_L (rubics_config* c, size_t count)
{
  for (; count; --count)
    move_pos<3, 4, 11, 7,
             0, 3,  4, 7>(c, false, 2);
}

void move_R (rubics_config* c, size_t count)
{
  for (; count; --count)
    move_pos<1, 6, 9, 5,
             1, 6, 5, 2>(c, false, 1);
}

void move_U (rubics_config* c, size_t count)
{
  for (; count; --count)
    move_pos<0, 1, 2, 3,
             0, 1, 2, 3>(c, false, 0);
}

void move_D (rubics_config* c, size_t count)
{
  for (; count; --count)
    move_pos<8, 9, 10, 11,
             4, 5,  6, 7>(c, false, 0);
}