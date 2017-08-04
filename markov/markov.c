/* markov.c - Generate random passwords using Markov chains
 * Copyright (C) 2017  Andrea Bolognani <andrea.bolognani@roundhousecode.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <limits.h>

#define TRUE  1
#define FALSE 0

typedef struct _node node;

struct _node {
    char *current;
    node **next;
    int *probability;

    int n_current;
    int n_next;
};

node*
node_new(const char *current,
         int n_next)
{
    node *self = NULL;
    int n_current = strlen(current);

    self = (node*) malloc(sizeof(node));
    if (!self)
        goto error;
    memset(self, 0, sizeof(node));

    self->current = strndup(current, n_current);
    if (!self->current)
        goto error;
    self->n_current = n_current;

    self->probability = calloc(n_next, sizeof(int));
    if (!self->probability)
        goto error;
    memset(self->probability, 0, n_next * sizeof(int));

    self->next = calloc(n_next, sizeof(node*));
    if (!self->next)
        goto error;
    memset(self->next, 0, n_next * sizeof(node*));
    self->n_next = n_next;

    return self;

 error:
    return NULL;
}

void
node_set_next(node *self,
              int index,
              node *other,
              int probability)
{
    if (index >= self->n_next)
        return;

    self->next[index] = other;
    self->probability[index] = probability;
}

node*
node_select_next(node *self)
{
    int choices[100];
    int pos;
    int index;
    int i;

    i = 0;
    for (index = 0; index < self->n_next; index++) {
        i += self->probability[index];
    }
    assert(i == 100);

    pos = 0;
    for (index = 0; index < self->n_next; index++) {
        for (i = 0; i < self->probability[index]; i++) {
            assert (pos < 100);
            choices[pos] = index;
            pos++;
        }
    }

    i = choices[random() % 100];
    assert(i >= 0 && i < self->n_next);

    return self->next[i];
}

char
node_select_current(node *self)
{
    int i;

    i = random() % self->n_current;

    return self->current[i];
}

static node*
build_model()
{
    node *start;
    node *lower;
    node *upper;
    node *digits;
    node *symbols;

    start = node_new("", 4);
    lower = node_new("qwertyuiopasdfghjklzxcvbnm", 4);
    upper = node_new("QWERTYUIOPASDFGHJKLZXCVBNM", 4);
    digits = node_new("1234567890", 4);
    symbols = node_new("!@#$%^&*()-_=+|\\;:,<.>/", 4);

    assert(start);
    assert(lower);
    assert(upper);
    assert(digits);
    assert(symbols);

    node_set_next(start, 0, lower, 25);
    node_set_next(start, 1, upper, 25);
    node_set_next(start, 2, digits, 25);
    node_set_next(start, 3, symbols, 25);

    node_set_next(lower, 0, lower, 20);
    node_set_next(lower, 1, upper, 30);
    node_set_next(lower, 2, digits, 30);
    node_set_next(lower, 3, symbols, 20);

    node_set_next(upper, 0, lower, 30);
    node_set_next(upper, 1, upper, 20);
    node_set_next(upper, 2, digits, 30);
    node_set_next(upper, 3, symbols, 20);

    node_set_next(digits, 0, lower, 30);
    node_set_next(digits, 1, upper, 30);
    node_set_next(digits, 2, digits, 20);
    node_set_next(digits, 3, symbols, 20);

    node_set_next(symbols, 0, lower, 30);
    node_set_next(symbols, 1, upper, 30);
    node_set_next(symbols, 2, digits, 30);
    node_set_next(symbols, 3, symbols, 10);

    return start;
}

int
main(int argc, char **argv)
{
    node *model;
    node *node;
    int i;
    int count = 0;

    srandom(time(NULL));
    model = build_model();
    assert(model);

    i = 0;
    node = model;
    while (node = node_select_next(node)) {
        printf("%c", node_select_current(node));

        i++;
        if (i >= 10) {
            printf("\n:");
            i = 0;
        }

        if (count >= 87000000)
            break;
        count++;
    }

    return 0;
}
