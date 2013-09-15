#include "hdrs/chan.hh"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

// TODO: remove
#include <iostream>
#include <cstring>

namespace chan {

    udp::udp (int port, const char *ipaddr)
        : sd(-1)
    {
        sd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sd == -1) {
            throw error("Creating socket", strerror(errno));
        }

        struct sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port)
        };

        if (ipaddr == NULL) {
            addr.sin_addr.s_addr = INADDR_ANY;
        } else {
            switch (inet_pton(AF_INET, ipaddr, &addr.sin_addr)) {
                case 0:
                    throw error("Invalid address", ipaddr);
                case -1:
                    throw error("Parsing address", strerror(errno));
                default:
                    break;
            }
        }

        if (bind(sd, (struct sockaddr *) &addr, sizeof(struct
                 sockaddr_in)) == -1) {
            throw error("Creating socket", strerror(errno));
        }
    }

    udp::~udp () {
        if (sd != -1) {
            close(sd);
        }
    }

    bool udp::read (read_info &info)
    {
        struct sockaddr_in from;
        socklen_t addrlen = sizeof(from);
        const char *tmp;

        ssize_t r = recvfrom(
            sd, info.data.buffer, info.data.size, 0,
            (struct sockaddr *)&from, &addrlen
        );

        if (r < 0) {
            throw error("Reading", strerror(errno));
        }
        if (r == 0) return false;

        info.len = r;
        tmp = inet_ntop(
            AF_INET, (const void *) &from.sin_addr, info.from_ip,
            INET_ADDRSTRLEN
        );
        if (tmp == NULL) {
            throw error("Sender address", strerror(errno));
        }
        return true;
    }

}
