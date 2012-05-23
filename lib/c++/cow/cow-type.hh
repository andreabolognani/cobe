#ifndef COW_TYPE_H
#define COW_TYPE_H

#include <cstddef>

namespace cow {

    template <typename TYPE>
    class Cow {
        private:
            struct Vessel {
                TYPE value;
                unsigned refcount;

                Vessel (const TYPE &v)
                    :   value(v),
                        refcount(1)
                {
                }
            };

        public:
            Cow ()
                : prev(NULL),
                  next(NULL),
                  vessel(NULL)
            {
            }

            Cow (const TYPE &v)
                : prev(NULL),
                  next(NULL),
                  vessel(new Vessel(v))
            {
            }

            Cow (const Cow<TYPE> &copy)
                : prev(const_cast<Cow<TYPE> *>(&copy)),
                  next(copy.next),
                  vessel(copy.vessel)
            {
                if (vessel) vessel->refcount ++;
                if (prev) {
                    if (prev->next) prev->next->prev = this;
                    prev->next = this;
                }
            }

            virtual ~Cow ()
            {
                if (prev) prev->next = next;
                if (next) next->prev = prev;
                drop_vessel();
            }

            Cow<TYPE> & operator= (const Cow<TYPE> &assign)
            {
                if (this != &assign && vessel != assign.vessel) {
                    Cow<TYPE> &other = const_cast<Cow<TYPE> &>(assign);

                    if (prev) prev->next = next;
                    if (next) next->prev = prev;
                    next = other.next;
                    if (next) next->prev = this;
                    other.next = this;
                    prev = &other;

                    drop_vessel();
                    vessel = other.vessel;
                    if (vessel) {
                        vessel->refcount ++;
                    }
                }

                return *this;
            }

            Cow<TYPE> & operator= (const TYPE &other)
            {
                access_for_change(&other);
                return *this;
            }

            operator const TYPE & () const
            {
                return vessel->value;
            }

        protected:
            TYPE & access_for_change (const TYPE * replace = NULL)
            {
                if (vessel == NULL || vessel->refcount > 1) {
                    if (vessel) {
                        if (prev) prev->next = next;
                        if (next) next->prev = prev;
                        drop_vessel();
                    }
                    vessel = new Vessel(replace ? *replace : TYPE());
                }
                return vessel->value;
            }

            TYPE & get_inner ()
            {
                return vessel->value;
            }

        private:
            void drop_vessel ()
            {
                if (vessel == NULL) return;
                vessel->refcount --;
                if (vessel->refcount == 0) {
                    delete vessel;
                }
            }

            Cow *prev;
            Cow *next;
            Vessel *vessel;
    };

}

#endif // COW_TYPE_H

