#include "timeout.h"

#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct tout {
    struct timeval last_update;
    struct timeval period;
};

tout_t tout_new (struct timeval *timeout)
{
    tout_t ret;

    ret = malloc(sizeof(struct tout));
    assert(ret != NULL);

    memcpy((void *)&ret->period, timeout, sizeof(struct timeval));
    tout_update(ret);

    return ret;
}

void tout_update (tout_t t)
{
    gettimeofday(&t->last_update, NULL);
}

int tout_expired (tout_t t)
{
    struct timeval now;
    struct timeval diff;

    gettimeofday(&now, NULL);
    timersub(&now, &t->last_update, &diff);

    return timercmp(&diff, &t->period, >);
}

void tout_del (tout_t t)
{
    free(t);
}

