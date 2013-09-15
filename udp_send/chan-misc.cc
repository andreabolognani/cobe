#include "hdrs/chan.hh"

#include <algorithm>
#include <arpa/inet.h>

namespace chan {

    error::error (const char *what, const char *why)
        :   std::runtime_error(why ? std::string(what) + ": " + why : why)
    {
    }

    bytes::bytes (unsigned n)
        :   size(n),
            buffer(new byte[n])
    {
        std::fill(buffer, buffer + n, 0);
    }

    bytes::~bytes () {
        delete[] buffer;
    }

    read_info::read_info (const unsigned buffer_size)
        :   from_ip(new char[INET_ADDRSTRLEN]),
            data(buffer_size)
    {
        std::fill(from_ip, from_ip + INET_ADDRSTRLEN, 0);
    }

    read_info::~read_info ()
    {
        delete[] from_ip;
    }

}
