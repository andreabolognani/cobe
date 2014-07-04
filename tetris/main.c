#include <stdio.h>
#include <stdlib.h>

#include "world.h"

int main(int argc, char **argv)
{
    world_t w;

    w = world_create(10);
    if (w == NULL) exit(EXIT_FAILURE);

    world_print(w, stdout);

    world_destroy(w);
    exit(EXIT_SUCCESS);
}
