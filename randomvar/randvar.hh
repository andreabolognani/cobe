#ifndef __defined_randvar_hh
#define __defined_randvar_hh

#include <vector>
#include <map>
#include <algorithm>
#include <initializer_list>
#include <cassert>
#include <ostream>
#include <iostream>

#include <cassert>
#include <utility>
#include <cstdlib>
#include <map>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <algorithm>

namespace prob
{
    template <class VTy, class Cmp = std::less<VTy> >
    class Dist : public std::map<VTy, double, Cmp>
    {
        public:
            Dist (const std::vector<VTy> &V, const std::vector<double> &P);
            Dist (std::initializer_list<VTy> V);
            Dist (const std::map<VTy, int> &freq, int N = 0);

        private:
            template <class Iter>
            void fill (Iter b, Iter e);
    };

    template <class VTy, class Cmp = std::less<VTy> >
    std::ostream & operator<< (std::ostream & out,
                               const Dist<VTy, Cmp> &D);

    template <class VTy0, class Cmp0 = std::less<VTy0> >
    double Pr (const Dist<VTy0, Cmp0> &dist, const VTy0 &value);

    template <class VTy, class Cmp = std::less<VTy> >
    class RandVar
    {
        public:
            RandVar (const Dist<VTy, Cmp> &dist);
            RandVar (const Dist<VTy, Cmp> &dist, VTy &value);

            const Dist<VTy, Cmp> &dist;

            void assign ();
            const VTy realize () const;
            void assign (const VTy &value);
            void reset ();

            operator VTy ();
            operator VTy () const;

            ~RandVar ();

        private:
            VTy *value;
    };

    #include "randvar-imp.hh"

}

#endif // __defined_randvar_hh

