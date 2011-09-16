#ifndef __defined_point_hpp
#define __defined_point_hpp

#include <set>
#include <ostream>

namespace data {

    class Point
    {
        public:
            Point (double x, double y);
            Point (Point const &p);
            Point ();

            Point & operator= (Point const &p);

            double get_x () const;
            double get_y () const;

            void print(std::ostream &out) const;

        private:
            double x;
            double y;

            friend class PointCmp;
    };

    struct PointCmp
    {
        bool operator() (const Point &p, const Point &q) const;
    };

    typedef std::set<Point, PointCmp> PointSet;

}

std::ostream & operator<< (std::ostream &out, data::Point const & p);

#endif // __defined_point_hpp
