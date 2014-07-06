#ifndef WORLD_H
#define WORLD_H

#include "types.h"
#include <stdio.h>

typedef struct world_s * world_t;

typedef enum {
    BLK_SQUARE, BLK_RIGHTL, BLK_LEFTL, BLK_RIGHTS, BLK_LEFTS, BLK_I
} block_t;

world_t world_create (uint cols, uint rows);
void world_print(world_t w, FILE *stream);
void world_insert (world_t w, block_t blk, uint col);
void world_shift(world_t w);
void world_destroy(world_t w);

#endif // WORLD_H
