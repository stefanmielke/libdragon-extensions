#include "mem_pool.h"

#define MEM_ALLOC(SIZE, MEMPOOL) ((MEMPOOL) ? mem_zone_alloc((MEMPOOL), (SIZE)) : malloc(SIZE))