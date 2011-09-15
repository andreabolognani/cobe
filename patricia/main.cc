#include "pat.hh"
#include <iostream>

int main ()
{
    Pat<float> p;

    p.add("Hello", 3.3);
    p.add("Sello", 4.3);
    p.add("Self control", 8.3);
    p.add("Self control indeed", 9.1);
    p.add("Self control indeed", 10.4);

    std::cout << p << std::endl;
}
