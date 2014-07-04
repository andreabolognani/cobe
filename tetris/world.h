#ifndef WORLD_H
#define WORLD_H

#include "types.h"
#include <stdio.h>

typedef struct world_s * world_t;

world_t world_create (uint cols);
void world_print(world_t w, FILE *stream);
void world_destroy(world_t w);

#endif // WORLD_H
