// last 7 bits of c.w contain the evaluation

#define SOLVED_CONFIG\
  ((rubics_config)(33296, 107843, 182390, 256937))

#define ADD_DIFF(c,d,X)                  \
  temp = c->X ^ d->X;                    \
  for (uint j = 0; j < 3; ++j)           \
  {                                      \
    diff += temp & EDGE_POS(j) ? 1 : 0;  \
    diff += temp & EDGE_ORI(j) ? 1 : 0;  \
  }                                      \
  for (uint j = 0; j < 2; ++j)           \
  {                                      \
    diff += temp & VERTEX_POS(j) ? 1 : 0;\
    diff += temp & VERTEX_ORI(j) ? 1 : 0;\
  }

uint compute_diff(rubics_config* c, int close_configs_index)
{
  uint diff = 0;
  uint temp;

  ADD_DIFF(c,CLOSE_CONFIGS[close_configs_index],x)
  ADD_DIFF(c,CLOSE_CONFIGS[close_configs_index],y)
  ADD_DIFF(c,CLOSE_CONFIGS[close_configs_index],z)
  ADD_DIFF(c,CLOSE_CONFIGS[close_configs_index],w)
  
  return diff;
}

uint evaluate(rubics_config* c)
{
  uint diff = 100;
  
  for (int i = 0; i < CLOSE_CONFIGS_COUNT; ++i)
    diff = min(diff, compute_diff(c, i));

  return diff;
}