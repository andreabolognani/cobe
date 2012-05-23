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

#ifndef COW_VECTOR_H
#define COW_VECTOR_H

#include <vector>
#include "cow-type.hh"

namespace cow {

    template <typename TYPE, unsigned SIZE>
    class Vector : public Cow< std::vector<TYPE> > {

        private:
            typedef std::vector<TYPE> Vec;
            typedef Cow<Vec> Super;

        public:
            Vector ()
            {
                Super::access_rw().resize(SIZE);
            }

            TYPE & operator[] (size_t pos)
            {
                return Super::access_rw()[pos];
            }

            const TYPE & operator[] (size_t pos) const
            {
                return Super::access_ro()[pos];
            }
    };

}

#endif // COW_VECTOR_H

