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

            /** Detach from any copy and get the internal object
             *
             * This method must be called from any class extending Cow
             * order get the internal object for modification.
             *
             * If some other instance of Cow is referring to the internal
             * object a copy is created; otherwise the internal object is
             * kept.
             *
             * @return A reference to the internal object ready for
             * modification.
             */
            TYPE & access_rw ()
            {
                return access_for_change(NULL);
            }

            /** Read-only access to the internal object.
             *
             * @return A constant reference to the internal object.
             */
            const TYPE & access_ro () const
            {
                return vessel->value;
            }

        private:
            Cow *prev;
            Cow *next;
            Vessel *vessel;

            TYPE & access_for_change (const TYPE * replace)
            {
                if (vessel == NULL || vessel->refcount > 1) {
                    Vessel * new_vessel;

                    if (!replace && vessel) {
                        replace = &vessel->value;
                    }
                    new_vessel = new Vessel(replace ? *replace : TYPE());
                    if (vessel) {
                        if (prev) prev->next = next;
                        if (next) next->prev = prev;
                        drop_vessel();
                    }
                    vessel = new_vessel;
                }
                return vessel->value;
            }

            void drop_vessel ()
            {
                if (vessel == NULL) return;
                vessel->refcount --;
                if (vessel->refcount == 0) {
                    delete vessel;
                }
            }
    };

}

#endif // COW_TYPE_H

