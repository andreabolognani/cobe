template <class VTy, class Cmp>
Dist<VTy, Cmp>::Dist (const std::vector<VTy> &V,
                      const std::vector<double> &P)
{
    assert( P.size() == V.size() );
    auto zip = boost::make_zip_iterator(
            boost::make_tuple(V.begin(), P.begin()),
            boost::make_tuple(V.end(), P.end())
    );
    fill(zip.begin(), zip.end());
}

template <class VTy, class Cmp>
Dist<VTy, Cmp>::Dist (std::initializer_list<VTy> V)
{
    struct BindProb
    {
        BindProb (const double p) : prob(p) {}

        const double prob;
        typedef std::pair<VTy, double> result_type;

        result_type operator() (const VTy &val) const
        {
            return std::make_pair(val, prob);
        }
    };

    typedef typename std::initializer_list<VTy>::iterator It;
    typedef boost::transform_iterator<BindProb, It> ProbIt;

    BindProb bp(1.0 / V.size());
    fill( ProbIt(V.begin(), bp), ProbIt(V.end(), bp) );
}

template <class VTy, class Cmp>
Dist<VTy, Cmp>::Dist (const std::map<VTy, int> &freq, int n)
{
    typedef typename std::map<VTy, int>::const_iterator It;

    struct BindProb
    {
        BindProb (const double p) : prob(p) {}

        const double prob;
        typedef std::pair<VTy, double> result_type;

        result_type operator() (const std::pair<VTy, int> &pair) const
        {
            return std::make_pair(pair.first, prob * pair.second);
        }
    };

    typedef boost::transform_iterator<BindProb, It> ProbIt;

    if (n == 0) {
        for (It i = freq.begin(); i != freq.end(); i ++) {
            n += i->second;
        }
    }

    BindProb bp(1.0 / n);
    fill( ProbIt(freq.begin(), bp), ProbIt(freq.end(), bp) );
}

template <class VTy, class Cmp>
template <class Iter>
void Dist<VTy, Cmp>::fill (Iter b, Iter e)
{
    for (auto i = b; i != e; i ++) {
        const VTy & value = i->first;
        auto j = find(value);
        if (j == this->end()) {
            insert( std::make_pair(value, i->second) );
        } else {
            (*this)[value] += i->second;
        }
    }
}

template <class VTy, class Cmp = std::less<VTy> >
std::ostream & operator<< (std::ostream & out, const Dist<VTy, Cmp> &D)
{
    out << "{";
    auto i = D.begin();
    bool end = ( i == D.end() );

    while (not end) {
        out << '(' << i->first << ", " << i->second;
        i ++;
        if (i == D.end()) {
            out << ')';
            end = true;
        } else {
            out << "), ";
        }
    }
    return out << "}";
}

template <class VTy, class Cmp = std::less<VTy> >
double Pr (const Dist<VTy, Cmp> &dist, const VTy &value)
{
    auto itm = dist.find(value);
    if (itm == dist.end())
        return 0;
    return itm->second;
}

template <class VTy, class Cmp>
RandVar<VTy, Cmp>::RandVar (const Dist<VTy, Cmp> &D)
    :   dist(D), value(NULL)
{
}

template <class VTy, class Cmp>
RandVar<VTy, Cmp>::RandVar (const Dist<VTy, Cmp> &D, VTy &V)
    :   dist(D), value(new VTy(V))
{
}

template <class VTy, class Cmp>
RandVar<VTy, Cmp>::~RandVar ()
{
    if (value) {
        delete value;
    }
}

template <class VTy, class Cmp>
const VTy RandVar<VTy, Cmp>::realize () const
{
    double rnd = static_cast<double>(random()) / RAND_MAX;
    auto i = dist.begin();

    while (i != dist.end()) {
        rnd -= i->second;
        if (rnd <= 0) {
            return i->first;
        }
        i ++;
    }
    return i->first;
}

template <class VTy, class Cmp>
void RandVar<VTy, Cmp>::assign ()
{
    assign(realize());
}

template <class VTy, class Cmp>
void RandVar<VTy, Cmp>::assign (const VTy &V)
{
    if (value == NULL) {
        value = new VTy(V);
    } else {
        *value = V;
    }
}

template <class VTy, class Cmp>
void RandVar<VTy, Cmp>::reset ()
{
    if (value != NULL) {
        delete value;
        value = NULL;
    }
}

template <class VTy, class Cmp>
RandVar<VTy, Cmp>::operator VTy ()
{
    if (value == NULL) {
        assign();
    }
    return *value;
}

template <class VTy, class Cmp>
RandVar<VTy, Cmp>::operator VTy () const
{
    if (value != NULL) {
        return *value;
    }
    return realize();
}

