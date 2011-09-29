#include "randvar.hh"

#include <iostream>
#include <map>
#include <vector>

int main (int argc, char *argv[])
{
    prob::Dist<int> D = {1, 2, 3, 3, 3};
    const prob::RandVar<int> X = D;

    std::map<int, int> freq;

    for (int i = 0; i < 500; i ++) {
        freq[X] ++;
    }

    prob::Dist<int> E = freq;
    std::cout << D << " -> " << E << std::endl;
}
