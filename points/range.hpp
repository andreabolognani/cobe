#ifndef __defined_range_hpp
#define __defined_range_hpp

class Range
{
    public:
        Range (double min, double max);
        Range (Range const &r);

        /* Val must be included in the continuous range [0, 1],
         * while the return value is included in [min, max].
         */
        double scale (double val);

        /* Val must be included in the continuous range [min, max],
         * while the return value is included in [0, 1]
         */
        double unscale (double val);

    private:
        double min;
        double max;

};

#endif // __defined_range_hpp

