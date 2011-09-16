#include <cassert>

#include "range.hpp"

Range::Range (double m, double M)
    :   min(m),
        max(M)
{
}

Range::Range (Range const &r)
    :   min(r.min),
        max(r.max)
{
}

double Range::scale (double val)
{
    assert(val >= 0.0 && val <= 1.0);
    return min + (max - min) * val;
}

double Range::unscale (double val)
{
    assert(min <= val && val <= max);
    return (val + max) / (max - min);
}
