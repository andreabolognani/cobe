#ifndef __defined_canvas_hpp
#define __defined_canvas_hpp

#include <SDL/SDL.h>

#include <stdexcept>
#include <string>
#include <list>

#include "point.hpp"
#include "range.hpp"

class Canvas
{
    public:

        struct Error : public std::runtime_error
        {
            Error (std::string const &msg) : std::runtime_error(msg) {}
        };

        Canvas (size_t width, size_t height, Range const &x,
                Range const &y);
        ~Canvas ();

        bool read (data::Point &p);
        void write (data::Point const &p);
        void write (std::list<data::Point> const &ps);

        void halt ();

    private:

        /* Ranges for logic canvas */
        Range xrange;
        Range yrange;

        /* Ranges for actual window */
        int width;
        int height;

        /* Scaling with respect to actual canvas size (pixels) */
        unsigned scale_x (double x);
        unsigned scale_y (double y);

        /* Given the coordinates of a mouse event, build a point relative
         * to the range */
        data::Point build_point (unsigned x, unsigned y);

        struct Sync {
            SDL_mutex *lock;
            SDL_cond *cond;
        };

        SDL_Surface *surface;
        SDL_Thread *thrd;

        Sync outsync;
        Sync insync;

        std::list<data::Point> out;
        std::list<data::Point> shown;

        /* Thread startup routines */
        friend int launch (void *canv);
        int run ();

        /* Check if work is needed */
        void check_work ();

        /* Generic request from GUI */
        void handle_event (SDL_Event &ev);

        /* Point entered from GUI */
        void entered_point (int x, int y);

        /* Request from "write" methods */
        void required_point (data::Point const &p);

        void draw_point (data::Point const &p);
        void draw_axis ();
        void update ();

        bool running;

};

#endif // __defined_canvas_hpp

