#include "world.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct world_s {
    uint32_t *cols;
    uint ncols;
};

world_t world_create (uint ncols)
{
    world_t out = NULL;

    out = malloc(sizeof(struct world_s));
    if (out == NULL) goto out;

    out->cols = malloc(sizeof(uint32_t) * ncols);
    if (out->cols == NULL) goto fail0;

    memset(out->cols, 0, sizeof(uint) * ncols);
    out->ncols = ncols;

    return out;
fail0:
    free(out);
out:
    return NULL;
}

void world_print (world_t world, FILE * stream)
{
    uint i;
    uint32_t msk;

    for (msk = 1; msk < 1<<31; msk <<= 1) {
        for (i = 0; i < world->ncols; i ++) {
            fputc((world->cols[i] & msk) ? 'x' : '-', stream);
        }
        fputc('\n', stream);
    }
}

void world_destroy (world_t w)
{
    free(w->cols);
    free(w);
}
