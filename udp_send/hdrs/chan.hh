#ifndef CHAN_HH
#define CHAN_HH

#include <stdexcept>
#include <string>

namespace chan {

    typedef unsigned char byte;

    struct bytes {
        const unsigned size;
        byte * const buffer;

        bytes (const unsigned size); 
        ~bytes ();
    };

    class error : public std::runtime_error {
        public:
            error(const char *what, const char *why=NULL);
    };

    struct read_info {
        char *from_ip;
        bytes data;
        unsigned len;

        read_info (const unsigned buffer_size);
        ~read_info();
    }; 

    class udp {
        public:
            explicit udp (int port, const char *ipaddr=NULL);
            ~udp ();

            bool read (read_info &info);

        private:
            int sd;
            udp (const udp &copy) {};
    };

}

#endif // CHAN_HH

