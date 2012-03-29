/* This testing program is a bit dirthy, sorry for that... it's just a
 * testing program after all. */

#include "esc.h"

#include <cstdlib>
#include <ctime>
#include <cstddef>
#include <cstdio>
#include <cerrno>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>

static const double MIN_RATIO = 0.2;
static const double BUF_REDUCE_RATIO = 0.8;

static size_t random_size (size_t max);
static std::list<size_t> copy_escaped (std::fstream &src, int outfd,
                                       size_t max_size);
static void read_and_compare (std::list<size_t> &chunks, int infd,
                              size_t max_size, std::fstream &orig);

int main (int argc, char **argv)
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <binary-file> <max-chunk>"
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string fname = argv[1];
    size_t max_size = atoi(argv[2]);

    if (max_size == 0) {
        std::cerr << "Invalid max-chunk parameter " << argv[2]
                  << std::endl;
    }

    FILE * escfile = fopen((fname + ".escaped").c_str(), "w+");

    std::fstream orig(argv[1], std::fstream::in | std::fstream::binary);
    std::list<size_t> chunks = copy_escaped(orig, fileno(escfile), max_size);

    fflush(escfile);
    rewind(escfile);
    orig.seekg(0, std::fstream::beg);

    read_and_compare(chunks, fileno(escfile), max_size, orig);

    fclose(escfile);
    exit(EXIT_SUCCESS);
}

static
void read_and_compare (std::list<size_t> &chunks, int infd,
                       size_t max_size, std::fstream &orig)
{
    max_size *= BUF_REDUCE_RATIO;
    char buffer[max_size];

    struct {
        size_t passed;
        size_t sim_failed;
        size_t failed;
    } stats;

    std::fill_n((uint8_t *)&stats, sizeof(stats), 0);

    while (!chunks.empty()) {
        size_t nxt_size, real_size;
        const char *estr;

        nxt_size = chunks.back();
        chunks.pop_back();

        std::cout << "Reading chunk:" << std::endl
                  << "\tExpected size: " << nxt_size << std::endl;
        if (max_size < nxt_size) {
            std::cout << "\tSimulating fail" << std::endl;
        }

        switch (real_size = esc_recv(infd, buffer, max_size)) {
            case -2:
                /* Advance of missing part */
                orig.seekg(nxt_size, std::fstream::cur);
                stats.sim_failed ++;
            case -1:
                estr = sys_errlist[errno];
                std::cout << "\tError: " << estr << std::endl;
                break;
            default:
                if (real_size != nxt_size) {
                    std::cout << "\tError: non-matching sizes"
                              << std::endl;
                } else {
                    char cmp_buf[real_size];
                    orig.readsome(cmp_buf, real_size);
                    if (std::equal(cmp_buf, cmp_buf+real_size, buffer)) {
                        stats.passed ++;
                        std::cout << "\tTest passed" << std::endl;
                    } else {
                        stats.failed ++;
                        std::cout << "\tTest not passed" << std::endl;
                    }
                }
        }
    }

    std::cout << "-------- STATISTICS --------" << std::endl
              << "\tpassed: " << stats.passed << std::endl
              << "\tsimulated failures: " << stats.sim_failed << std::endl
              << "\tactual failures: " << stats.failed << std::endl;
}

static
std::list<size_t> copy_escaped (std::fstream &src, int outfd,
                                size_t max_size)
{
    char buffer[max_size + 1];
    size_t n;
    std::list<size_t> chunks;

    while ((n = src.readsome(buffer, random_size(max_size))) > 0) {
        chunks.push_front(n);
        esc_send(outfd, buffer, n);
    }

    return chunks;
}

static
size_t random_size (size_t max)
{
    double rand01 = static_cast<double>(random()) / RAND_MAX;

    size_t min = max * MIN_RATIO;
    return min + (max - min) * rand01;
}

