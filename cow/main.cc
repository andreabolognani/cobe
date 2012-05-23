#include <iostream>
#include <string>
#include "cow-vector.hh"

static const unsigned N = 5;

static void init (cow::Vector<int, N> &v, int from)
{
    for (unsigned i = 0; i < N; i ++) {
        v[i] = from + i;
    }
}

static void print (const char * name, const cow::Vector<int, N> &v)
{
    const std::vector<int> & inner = v;

    std::cout << "Dumping vector \"" << name << "\": " << std::endl
              << "\tInternal address: " << &inner << std::endl
              << "\tValues: ";
    for (unsigned i = 0; i < N; i ++) {
        std::cout << " " << v[i];
    }
    std::cout << std::endl;
}

int main ()
{
    cow::Vector<int, N> A;

    print("A", A);

    std::cout << std::string(80, '-') << std::endl;
    std::cout << "> Initializing A" << std::endl;
    init(A, 5);
    print("A", A);

    std::cout << std::string(80, '-') << std::endl;
    std::cout << "> Declaring B = A" << std::endl;
    cow::Vector<int, N> B = A;
    print("B", B);
    print("A", A);

    std::cout << std::string(80, '-') << std::endl;
    std::cout << "> Declaring C; Assigning C = A" << std::endl;
    cow::Vector<int, N> C;
    C = A;
    print("A", A);
    print("B", B);
    print("C", C);

    std::cout << std::string(80, '-') << std::endl;
    std::cout << "> Assigning B = C" << std::endl;
    B = C;
    print("A", A);
    print("B", B);
    print("C", C);

    std::cout << std::string(80, '-') << std::endl;
    std::cout << "> Modifying C" << std::endl;
    C[2] = 500;
    print("A", A);
    print("B", B);
    print("C", C);

    std::cout << std::string(80, '-') << std::endl;
    std::cout << "> Assigning A = C" << std::endl;
    A = C;
    print("A", A);
    print("B", B);
    print("C", C);


    return 0;
}

