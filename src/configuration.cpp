#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "configuration.hpp"

void cmd_switch(const std::string& cmd, rubics_config* c)
{
  // number of turns
  auto count = 1;
  if (cmd.size() > 1)
  {
    if (cmd[1] == '2')
      count = 2;
    else
      count = 3;
  }

  // which command
  switch (cmd[0])
  {
    case 'F': move_F(c, count);
    case 'B': move_B(c, count);
    case 'L': move_L(c, count);
    case 'R': move_R(c, count);
    case 'U': move_U(c, count);
    case 'D': move_D(c, count);
  }
}

rubics_config parse_rubics(std::string line)
{
  std::vector<std::string> tokens;

  // split into tokens
  std::istringstream iss(line);
  std::copy(
    std::istream_iterator<std::string>(iss),
    std::istream_iterator<std::string>(),
    std::back_inserter(tokens));
  
  // start with solved rubics cube
  auto config = SOLVED_CONFIG;

  // apply commands one by one
  for(const auto& cmd : tokens)
    cmd_switch(cmd, &config);

  return config;
}

// move implementations

template <int EA, int EB, int EC, int ED,
          int VA, int VB, int VC, int VD>
void move (rubics_config* c)
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
  // edge orientations
  auto eo1 = get_edge_ori<EA>(c);
  auto eo2 = get_edge_ori<EB>(c);
  auto eo3 = get_edge_ori<EC>(c);
  auto eo4 = get_edge_ori<ED>(c);
  set_edge_ori<EA>(c, eo4);
  set_edge_ori<EB>(c, eo1);
  set_edge_ori<EC>(c, eo2);
  set_edge_ori<ED>(c, eo3);
  // vertex orientations
  auto vo1 = get_vertex_ori<VA>(c);
  auto vo2 = get_vertex_ori<VB>(c);
  auto vo3 = get_vertex_ori<VC>(c);
  auto vo4 = get_vertex_ori<VD>(c);
  set_vertex_ori<EA>(c, vo4);
  set_vertex_ori<EB>(c, vo1);
  set_vertex_ori<EC>(c, vo2);
  set_vertex_ori<ED>(c, vo3);
}

void move_F (rubics_config* c, size_t count)
{
  move<0, 5, 8, 4,
       0, 1, 5, 4>(c);
}

void move_B (rubics_config* c, size_t count)
{
  move<2, 7, 10, 6,
       2, 3, 7,  6>(c);
}

void move_L (rubics_config* c, size_t count)
{
  move<3, 4, 11, 7,
       0, 4, 7,  3>(c);
}

void move_R (rubics_config* c, size_t count)
{
  move<1, 6, 9, 5,
       1, 2, 6, 5>(c);
}

void move_U (rubics_config* c, size_t count)
{
  move<0, 3, 2, 1,
       0, 3, 2, 1>(c);
}

void move_D (rubics_config* c, size_t count)
{
  move<8, 9, 10, 11,
       4, 5, 6,  7>(c);
}