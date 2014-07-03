#include <stdio.h>
#include <stdlib.h>

typedef unsigned uint;

typedef struct ctx_s {
    char *buffer;
    uint bits;
} ctx_t;

static int create_ctx (ctx_t *ctx, uint bits)
{
    const uint n = (bits + 1) * 2;
    void * mem = malloc(sizeof(char) * n);
    int i;

    if (mem == NULL) {
        return -1;
    }
    ctx->buffer = (char *)mem;
    ctx->bits = bits;

    ctx->buffer[n - 1] = '\0';
    ctx->buffer[0] = '|';
    for (i = 1; i < n - 1; i ++) {
        ctx->buffer[i] = (i % 2) ? '0' : '|';
    }

    return 0;
}

static void free_ctx (ctx_t *ctx)
{
    free(ctx->buffer);
}

static void set_value (ctx_t *ctx, uint value)
{
    uint i;
    
    for (i = 0; i < ctx->bits; i ++) {
        ctx->buffer[2 * i + 1] = '0' + !!(value & (1 << i));
    }
}

int main (int argc, char *argv[])
{
    ctx_t ctx;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <value>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    create_ctx(&ctx, 32);
    set_value(&ctx, atoi(argv[1]));
    printf("%s\n", ctx.buffer);
    free_ctx(&ctx);

    exit(EXIT_SUCCESS);
}
