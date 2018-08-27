#ifndef BIT_MASK
#define BIT_MASK

#define EDGE_POS(N) (15 << ((N) * 4))
#define VERTEX_POS(N) (7 << (12 + (N) * 3))
#define EDGE_ORI(N) (1 << (18 + (N)))
#define VERTEX_ORI(N) (3 << (21 + (N) * 2))

#endif