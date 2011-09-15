#include "pat.hh"

#include <iostream>
#include <algorithm>
#include <cassert>

namespace {

    /** The CoreInfo structure contains information about the Core we are
     *  going to use as root for a certain prefix.
     *
     * Once we localized the core to use, this structure will be returned
     * and different actions will be taken depending on the use.
     */
    struct CoreInfo {

        Core * core;            /**< The core we are referring to; */

        std::size_t common;     /**< Length of the common prefix between
                                 *   the core and the new symbol string.
                                 */
        enum Use {
            OWN,                /**< The core is owned by the newcoming
                                 *   string;
                                 */
            FIRST,              /**< Same as OWN, but the core is the head
                                 * of the level;
                                 */
            SHARE               /**< The selected core was already
                                 * present. */
        } use;                  /**< How we should use the core; */

        enum ForkType {
            UNKNOWN,            /**< Not analysed yet; */
            SAME_NAME,          /**< The string and the prefix are exactly
                                 * the same; */
            P0_EXTENDS_P1,      /**< The prefix of the core extends the
                                 * new string; */
            P1_EXTENDS_P0,      /**< The new string extends the prefix of
                                 * the core; */
            COMMON_PREFIX,      /**< A common root, different suffixes; */
            UNRELATED           /**< Nothing in common. */
        } fork_type;            /**< How strings are related */

        /** Constructor allowing to specify the use.
         *
         * @param c The core we are referring to;
         * @param u The required use of the core.
         */
        CoreInfo (Core *c, CoreInfo::Use u);

        /** Constructor allowing to specify a string to compare with the
         * one embedded in the core.
         *
         * @param c The core we are referring to;
         * @param p The prefix.
         */
        CoreInfo (Core *c, const std::string &p);

    };

    CoreInfo::CoreInfo (Core *c, CoreInfo::Use u)
        :   core(c), use(u), fork_type(SAME_NAME)
    {
        assert( u == OWN || u == FIRST );
    }

    CoreInfo::CoreInfo (Core *c, const std::string &p)
        :   core(c), use(CoreInfo::SHARE), fork_type(CoreInfo::UNKNOWN)
    {
        const char * raw0 = core->prefix.c_str();
        const char * raw1 = p.c_str();

        /* Note: here we already know that raw0 and raw1 have a common
         * prefix. What we don't know is how the common part is shaped
         * w.r.t. the two strings. */

        for (unsigned i = 0; fork_type != UNKNOWN; i ++) {
            switch ((!!raw0[i]) | ((!!raw1[i]) << 1)) {
                case 0:
                    /* We reached the end simultaneously */
                    fork_type = SAME_NAME;
                    break;
                case 1:
                    /* String 0 ended while scanning */
                    fork_type = P0_EXTENDS_P1;
                    break;
                case 2:
                    /* String 1 ended while scanning */
                    fork_type = P1_EXTENDS_P0;
                    break;
                default:
                    /* Comparison */
                    if (raw0[i] != raw1[i]) {
                        fork_type = COMMON_PREFIX;
                    }
            }

        }
    }

    CoreInfo get_core (Core *c, const std::string &p, void *o)
    {
        if (c == NULL || p[0] < c->prefix[0]) {
            return CoreInfo( new Core(p, o, c), CoreInfo::FIRST );
        }

        Core *prev = NULL;
        /* We keep moving among the ordered neighbors of the same level.
         * This is necessary to keep ordering */
        do {
            prev = c;
            c = c->next;
        } while ((c != NULL) && !(p[0] < c->prefix[0]));

        if (prev->prefix[0] < p[0]) {
            return CoreInfo( prev->next = new Core(p, o, c),
                             CoreInfo::OWN );
        } else {
            return CoreInfo( prev, p );
        }
    }

}

Core * Core::append (Core *c, const std::string &p1, void *o)
{
    CoreInfo root = get_core(c, p1, o);

    if (root.use == CoreInfo::OWN) return c;
    else if (root.use == CoreInfo::FIRST) return root.core;

    std::string & p0 = root.core->prefix;

    /* We have to disambiguate */

    if (root.common == p0.length()) {

        if (root.common == p1.length()) {
            /* Exactly the same string! Replace content */
            root.core->value = o;
        } else {
            /* The p1 prefix extends p0, we recursively send it
             * down with the suffix of the prefix */
            root.core->sub = append(root.core->sub,
                                    p1.substr(root.common), o);
        }

    } else if (root.common == p1.length()) {

        /* The p1 prefix is like p0, but shorter */
        root.core->sub = append(root.core->sub, p0, root.core->value);
        root.core->value = o;
        p0.resize(root.common);

    } else {

        /* The p0 and the p1 prefixes have a common part */
        root.core->sub = append(root.core->sub, p0.substr(root.common),
                                root.core->value);
        root.core->sub = append(root.core->sub, p1.substr(root.common), o);
        root.core->value = NULL;
        p0.resize(root.common);

    }

    return c;
}

Core::Core (const std::string &p, void *v, Core *nx, Core *sb)
    :   prefix(p), value(v), next(nx), sub(sb)
{
}

Core::~Core ()
{
    if (next) delete next;
    if (sub) delete sub;
}

