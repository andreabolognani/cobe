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

