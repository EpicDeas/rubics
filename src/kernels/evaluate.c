// these numbers were obtained from the host constant SOLVED_CONFIG
#define SOLVED_CONFIG\
  ((rubics_config)(33296, 107843, 182390, 256937))

#define ADD_DIFF(X)                      \
  temp = c->X ^ SOLVED_CONFIG.X;         \
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

uint evaluate(rubics_config* c)
{
  uint diff = 0;
  uint temp;

  ADD_DIFF(x)
  ADD_DIFF(y)
  ADD_DIFF(z)
  ADD_DIFF(w)
  
  return diff;
}