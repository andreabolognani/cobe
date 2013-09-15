#include "hdrs/chan.hh"

#include <iostream>
#include <unistd.h>

namespace {

    void run () {
        chan::udp Out(1025);
        chan::read_info Info(128);

        while (Out.read(Info)) {
            std::cout << "Received: " << Info.len << " bytes from "
                      << Info.from_ip << std::endl
                      << "--> " << Info.data.buffer
                      << std::endl << std::endl;
        }
    }

}

int main ()
{
    try {
        run();
    } catch (chan::error &e) {
        std::cerr << e.what() << std::endl;
    }
}
