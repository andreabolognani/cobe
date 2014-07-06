#include "world.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint32_t cells;
    uint32_t solid;
} column_t;

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

static void col_shift (column_t *col)
{
    col->cells = (col->cells & col->solid)
               | ((col->cells & ~col->solid) >> 1);
}

static void col_solidify (column_t *col)
{
    const uint32_t shift = col->solid + 1;
    uint32_t block = col->cells / shift;

    if (block > 0) {
        uint32_t mask = 1;
        while (block & mask) {
            mask <<= 1;
        }
        col->solid |= (mask - 1) * shift;
    }
}

void world_print (world_t w, FILE * stream)
{
    uint i;
    uint32_t msk;
    const uint32_t max = 1 << w->nrows;

    for (msk = max; msk > 0; msk >>= 1) {
        for (i = 0; i < w->ncols; i ++) {
            fputc((w->cols[i].cells & msk)
                  ? (w->cols[i].solid & msk) ? 'x' : 'o'
                  : '.',
                stream
            );
        }
        fputc('\n', stream);
    }
}

void world_shift (world_t w)
{
    uint i;

    for (i = 0; i < w->ncols; i ++) {
        column_t *col = w->cols + i;
        col_solidify(col);
        col_shift(col);
    }
}

void world_destroy (world_t w)
{
    free(w->cols);
    free(w);
}
