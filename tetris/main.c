#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "world.h"

static void shift (world_t w, uint N)
{
    uint i;

    for (i = 0; i < N; i ++) {
        world_print(w, stdout);
        sleep(1);
        world_shift(w);
        fputs("-----------------------\n", stdout);
    }
}

int main(int argc, char **argv)
{
    world_t w;

    w = world_create(7, 10);
    if (w == NULL) exit(EXIT_FAILURE);

    world_insert(w, BLK_I, 0);
    shift(w, 5);
    world_insert(w, BLK_SQUARE, 3);
    shift(w, 5);
    world_insert(w, BLK_LEFTL, 4);
    shift(w, 5);
    world_insert(w, BLK_SQUARE, 3);
    shift(w, 5);
    world_insert(w, BLK_SQUARE, 3);
    shift(w, 5);

    world_destroy(w);
    exit(EXIT_SUCCESS);
}
