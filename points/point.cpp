#include "point.hpp"

namespace data {

    Point::Point (double _x, double _y)
        :   x(_x),
            y(_y)
    {
    }

    Point::Point (Point const &p)
        :   x(p.x),
            y(p.y)
    {
    }

    Point::Point ()
        :   x(0.0),
            y(0.0)
    {
    }

    Point & Point::operator= (Point const &p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }

    double Point::get_x () const
    {
        return x;
    }

    double Point::get_y () const
    {
        return y;
    }

    void Point::print (std::ostream &out) const
    {
        out << '(' << x << ',' << y << ')';
    }

    bool PointCmp::operator() (const Point &p, const Point &q) const
    {
        return p.x < q.x && p.y < q.y;
    }

}

std::ostream & operator<< (std::ostream &out, data::Point const & p)
{
    p.print(out);
    return out;
}
