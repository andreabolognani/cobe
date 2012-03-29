#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <sys/time.h>

typedef struct tout * tout_t;

tout_t tout_new (struct timeval *timeout); 

void tout_update (tout_t t);

int tout_expired (tout_t t);

void tout_del (tout_t t);

#endif // TIMEOUT_H

