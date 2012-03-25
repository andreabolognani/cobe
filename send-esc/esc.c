#include "esc.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static const uint8_t BYTE_ESC = 0x1b;
static const uint8_t BYTE_EOT = 0x04;

typedef struct {
    uint32_t msg_len;
} header_t;

static void header_set_len (header_t *hdr, size_t len)
{
    hdr->msg_len = htonl(len);
}

static size_t header_get_len (header_t *hdr)
{
    return ntohl(hdr->msg_len);
}

static int send_header (int fd, const header_t *hdr)
{
    return write(fd, (const void *)hdr, sizeof(header_t));
}

static ssize_t recv_header (int fd, header_t *hdr)
{
    size_t target;
    uint8_t *bytes;

    target = sizeof(header_t);
    bytes = (uint8_t *)hdr;

    while (target > 0) {
        ssize_t n;

        n = read(fd, (void *)bytes, target);
        if (n <= 0) return n;

        target -= n;
        bytes += n;
    }

    return sizeof(header_t);
}

static void flush_message (int fd)
{
    int state;
    uint8_t byte;

    state = 0;
    for (;;) {
        if (read(fd, (void *)&byte, sizeof(byte)) <= 0) return;

        switch (state) {
            case 0: /* Last byte was normal */
                if (byte == BYTE_EOT) return;
                if (byte == BYTE_ESC) state ++;
                break;
            case 1: /* Last byte was escape */
                state --;
        }
    }
}

ssize_t esc_send (int fd, const void *buffer, size_t src_size)
{
    const uint8_t *src;
    size_t dst_size = (src_size * 2) + 1;
    uint8_t dst[dst_size];
    unsigned i, j, esc;
    header_t hdr;

    header_set_len(&hdr, src_size);
    send_header(fd, &hdr);

    src = (const uint8_t *)buffer;
    esc = 0;
    for (i = j = 0; j < src_size; i ++) {
        uint8_t b = src[j];

        if (!esc && (b == BYTE_ESC || b == BYTE_EOT)) {
            dst[i] = BYTE_ESC;
            esc = 1;
        } else {
            dst[i] = b;
            j ++;
            esc = 0;
        }
    }
    dst[i ++] = BYTE_EOT;

    return write(fd, dst, i);
}

ssize_t esc_recv (int fd, void *buffer, size_t size)
{
    uint8_t *check;
    int i, j, esc;
    header_t hdr;
    size_t target, cumulated;

    if (recv_header(fd, &hdr) <= 0) return -1;

    /* Note: we need room for the EOT byte, so minimum length will need
     * one extra byte */
    target = header_get_len(&hdr) + 1;
    errno = 0;
    if (size < target) {
        flush_message(fd);
        errno = ENOBUFS;
        return -2;
    }

    check = (uint8_t *)buffer;
    esc = 0;
    cumulated = 0;
    while (target > 0) {
        ssize_t n;

        n = read(fd, (void *)check, target);
        if (n <= 0) return n;

        for (i = j = 0; i < n; i ++) {
            uint8_t b = check[i];

            if (esc) {
                check[j ++] = b;
                esc = 0;
            } else switch (b) {
                case BYTE_ESC:
                    esc = 1;
                    break;
                case BYTE_EOT:
                    return cumulated + j;
                default:
                    check[j ++] = b;
            }
        }

        check += j;
        target -= j;
        size -= j;
        cumulated += j;
    }

    /* This is here just for safety. The function should return inside the
     * cycle, and if we got here the protocol is just screwed */
    return -1;
}

