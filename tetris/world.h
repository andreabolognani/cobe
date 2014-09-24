#ifndef WORLD_H
#define WORLD_H

#include "types.h"
#include <stdio.h>

typedef struct world_s * world_t;

typedef enum {
    BLK_NONE   = 0,
    BLK_SQUARE = 1,
    BLK_RIGHTL = 2,
    BLK_LEFTL  = 3,
    BLK_RIGHTS = 4,
    BLK_LEFTS  = 5,
    BLK_I      = 6
} block_kind_t;

world_t world_create (uint cols, uint rows);
void world_print(world_t w, FILE *stream);
void world_insert (world_t w, block_kind_t blk, uint col);
void world_shift(world_t w);
void world_destroy(world_t w);

#endif // WORLD_H
