// last 7 bits of c.w contain the evaluation

#define EVAL_MASK (127 << 25)

#define WRITE_EVAL(c, eval)\
  c->w = (c->w & ~EVAL_MASK) | (eval << 25);

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

uint compute_diff(
  rubics_config* c,
  __constant const rubics_config* d)
{
  uint diff = 0;
  uint temp;

  ADD_DIFF(c,d,x)
  ADD_DIFF(c,d,y)
  ADD_DIFF(c,d,z)
  ADD_DIFF(c,d,w)
  
  return diff;
}

void evaluate(rubics_config* c)
{
  uint diff = 100;
  
  for (int i = 0; i < CLOSE_CONFIGS_COUNT; ++i)
    diff = min(diff, compute_diff(c, &CLOSE_CONFIGS[i]));

  WRITE_EVAL(c, diff)
}