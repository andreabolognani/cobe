#ifndef __defined_pat_hh
#define __defined_pat_hh

#include <string>
#include <algorithm>
#include <ostream>
#include <iomanip>
#include <queue>

#include <iostream>

struct Core {

    std::string prefix;
    void *value;

    Core *next;
    Core *sub;

    Core (const std::string &pfx, void *value, Core *next = NULL,
          Core *sub = NULL);

    ~Core ();

    static Core * append (Core *c, const std::string &p, void *o);

    template <typename T> static
    void print (const Core & c, std::ostream &out);

};

static inline
std::ostream & operator<< (std::ostream &out, const Core &c)
{
    return out << "core_" << std::dec << static_cast<const void *>(&c);
}

template <typename T>
void Core::print (const Core &c, std::ostream &out)
{
    out << '\t'<< c << " [label=\"{"
        << c.prefix << '|';
    if (c.value) {
        out << * static_cast<const T *>(c.value);
    }
    out << "}\"]" << std::endl;

    if (c.next) {
        const Core &next = *c.next;

        out << "\t{ rank=same; " << c << ';' << next << "; }"
            << std::endl;

        Core::print<T>(next, out);
    }

    Core *scan = c.sub;
    while (scan != NULL) {
        const Core &scanref = *scan;

        out << "\t" << c << " -> " << scanref << std::endl;
        Core::print<T>(scanref, out);

        scan = scan->next;
    }
}

template <typename T>
class Pat
{
    public:
        Pat () : core(NULL) {
        }

        ~Pat () {
            if (core)
                delete core;
        }

        void add (const std::string &pfx, const T &o) {

            if (pfx.length() == 0) {
                throw "Zero length?";
            }

            core = Core::append(core, pfx, new T(o));
        }

        void print (std::ostream &out) const {
            if (core) {
                out << "digraph Patricia {" << std::endl
                    << "\tnode [shape=record];" << std::endl;

                Core::print<T>(*core, out);
                out << "}";
            } else {
                out << "Empty tree";
            }
        }

    private:
        Core *core;
};

template <typename T>
std::ostream & operator<< (std::ostream &out, const Pat<T> &p)
{
    p.print(out);
    return out;
}

#endif // __defined_pat_hh

