#include "timeout.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
    tout_t T;
    int ntests = 5;
    struct timeval tout = {
        .tv_sec = 1,
        .tv_usec = 500000
    };

    T = tout_new(&tout);
    while (ntests) {
        while (!tout_expired(T)) {
            usleep(250000);
            putchar('.');
            fflush(stdout);
        }
        printf("expired\n");
        ntests --;
        tout_update(T);
    }

    exit(EXIT_SUCCESS);
}

