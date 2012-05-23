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
                Super::access_for_change().resize(SIZE);
            }

            TYPE & operator[] (size_t pos)
            {
                Vec &inner = Super::get_inner();
                std::vector<TYPE> &vec = Super::access_for_change(&inner);
                return vec[pos];
            }

            const TYPE & operator[] (size_t pos) const
            {
                return static_cast<const Vec &>(*this)[pos];
            }
    };

}

#endif // COW_VECTOR_H

