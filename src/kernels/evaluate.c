// last 7 bits of c.w contain the evaluation

#define EVAL_MASK (127 << 25)

#define WRITE_EVAL(c, eval)\
  c->w = (c->w & ~EVAL_MASK) | (eval << 25);

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

void evaluate(rubics_config* c)
{
  uint diff = 0;
  uint temp;

  ADD_DIFF(x)
  ADD_DIFF(y)
  ADD_DIFF(z)
  ADD_DIFF(w)
  
  WRITE_EVAL(c, diff)
}