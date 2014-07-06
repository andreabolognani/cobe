#include "world.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef uint64_t column_t;

struct world_s {
    column_t *cols;
    uint32_t nrows;
    uint ncols;
};

world_t world_create (uint ncols, uint nrows)
{
    world_t out = NULL;

    if (nrows > 31) {
        fputs("Only up to 31 rows allowed\n", stderr);
        goto fail;
    }

    out = malloc(sizeof(struct world_s));
    if (out == NULL) goto fail;

    out->cols = malloc(sizeof(column_t) * ncols);
    if (out->cols == NULL) goto fail0;

    memset(out->cols, 0, sizeof(uint) * ncols);
    out->ncols = ncols;
    out->nrows = nrows;

    return out;
fail0:
    free(out);
fail:
    return NULL;
}

uint32_t col_shift (uint32_t limit, uint32_t col)
{
    return log2(col) + 1;
}

void world_print (world_t w, FILE * stream)
{
    uint i;
    uint32_t msk;
    const uint32_t max = 1 << w->nrows;

    for (msk = max; msk > 0; msk >>= 1) {
        for (i = 0; i < w->ncols; i ++) {
            fputc((w->cols[i] & msk) ? 'x' : '-', stream);
        }
        fputc('\n', stream);
    }
}

void world_destroy (world_t w)
{
    free(w->cols);
    free(w);
}
