/*
 * Copyright (c) 2012 Giovanni Simoni
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 600
#undef  _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct {
    struct sockaddr_in addr;
    int fd;
} sock_data_t;

static inline
int sock_data_cmp (const sock_data_t *s1, const sock_data_t *s2)
{
    return memcmp(&s1->addr, &s2->addr, sizeof(struct sockaddr_in));
}

static inline
int sock_data_equal (const sock_data_t *s1, const sock_data_t *s2)
{
    return sock_data_cmp(s1, s2) == 0;
}

static inline
int sock_data_init (sock_data_t *s, const char *IPaddr, int port)
{
    s->addr.sin_family = AF_INET;
    s->addr.sin_port = htons(port);
    s->fd = -1;
    if (IPaddr == NULL) {
        s->addr.sin_addr.s_addr = INADDR_ANY;
        return 1;
    } else {
        /* NOTE: returns 0 on error */
        return inet_pton(AF_INET, IPaddr, &s->addr.sin_addr);
    }
}

static inline
const char * sock_data_ipstring (const sock_data_t *s, char * strrep)
{
    return inet_ntop(AF_INET, (const void *) &s->addr.sin_addr, strrep,
                     INET_ADDRSTRLEN);
}

/* -- Internal functions ---------------------------------------------- */

static int tcp_connect (sock_data_t *sd, int *e);
static int tcp_serve (sock_data_t *sd, int backlog, int *e);

static const unsigned DEFAULT_BACKLOG = 50;

/* -- Interface exported symbols -------------------------------------- */

static const unsigned BUFLEN = INET_ADDRSTRLEN > 256 ?
                               INET_ADDRSTRLEN : 256;

static void client ()
{
    sock_data_t self;
    char buffer[BUFLEN];
    int e;

    sock_data_init(&self, "127.0.0.1", 9000);

    if (tcp_connect(&self, &e)) {
        strerror_r(e, buffer, BUFLEN);
        fprintf(stderr, "Connecting to server: %s\n", buffer);
        return;
    }

    write(self.fd, "hello world", strlen("hello world"));
    close(self.fd);
}

static void server ()
{
    sock_data_t serv;
    sock_data_t clnt;
    char buffer[BUFLEN];
    ssize_t R;
    int e;

    sock_data_init(&serv, NULL, 9000);

    if (tcp_serve(&serv, DEFAULT_BACKLOG, &e) < 0) {
        strerror_r(e, buffer, BUFLEN);
        fprintf(stderr, "Creating server: %s\n",
                buffer);
        return;
    }

    socklen_t addrlen = sizeof(struct sockaddr_in);
    clnt.fd = accept(serv.fd, (struct sockaddr *)&clnt.addr,
                     &addrlen);
    if (clnt.fd == -1) {
        strerror_r(errno, buffer, BUFLEN);
        fprintf(stderr, "No accept: %s\n", buffer);
        close(serv.fd);
        return;
    }

    printf("Got connection from %s\n",
           sock_data_ipstring(&clnt, buffer));

    memset((void *)buffer, 0, BUFLEN);
    while ((R = read(clnt.fd, buffer, BUFLEN - 1)) > 0) {
        buffer[R] = 0;
        printf("> %s\n", buffer);
    }
    if (R == 0) {
        fprintf(stderr, "End of transmission\n");
    } else {
        strerror_r(errno, buffer, BUFLEN);
        fprintf(stderr, "Error in transmission: %s\n", buffer);
    }
    close(clnt.fd);
    close(serv.fd);
}

int main (int argc, char **argv)
{
    pid_t P = fork();

    if (P == 0) {
        // child
        sleep(2); // This is a dirty way of giving the server time to be
                  // running
        client();
        printf("Bye 0\n");
    } else {
        // father
        server();
        waitpid(P, NULL, 0);
        printf("Bye 1\n");
    }

    return 0;
}


/* -- Internal functions ---------------------------------------------- */

/* Returns 0 on success, non-0 on failure. Upon failure `e` is setted to
 * errno */
static
int tcp_connect (sock_data_t *sd, int *e)
{
    int fd;

    if (sd->fd != -1) {
        /* Already connected */
        return 0;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        if (e) *e = errno;
        return -1;
    }

    if (connect(fd, (struct sockaddr *) &sd->addr,
            sizeof(struct sockaddr_in)) == -1) {
        if (e) *e = errno;
        close(fd);
        return -2;
    }
    sd->fd = fd;

    return 0;
}

static
int tcp_serve (sock_data_t *sd, int backlog, int *e)
{
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        if (e) *e = errno;
        return -1;
    }

    if (bind(fd, (struct sockaddr *) &sd->addr,
            sizeof(struct sockaddr_in))) {
        if (e) *e = errno;
        close(fd);
        return -2;
    }

    if (listen(fd, backlog) == -1) {
        if (e) *e = errno;
        close(fd);
        return -3;
    }
    sd->fd = fd;

    return 0;
}

