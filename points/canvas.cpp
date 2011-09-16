#include <cassert>
#include <iostream>
#include <sstream>

#include <SDL/SDL_gfxPrimitives.h>

#include "canvas.hpp"

static const Uint32     DOT_COLOR       = 0xff0000aa;
static const Uint32     LAB_COLOR       = 0xffffffff;
static const Uint32     AXIS_COLOR      = 0xffffffaa;

static const int        DEPTH           = 32;
static const unsigned   SLEEP           = 1000 / 30;

int launch (void *canv)
{
    return static_cast<Canvas *>(canv)->run();
}

Canvas::Canvas (size_t w, size_t h, Range const &x, Range const & y)
    :   xrange(x),
        yrange(y),
        width(w),
        height(h),
        thrd(NULL),
        running(false)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Error("Initializing SDL");
    }
    surface = SDL_SetVideoMode(w, h, DEPTH, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (surface == NULL) {
        throw Error("Building surface");
    }
    outsync.lock = SDL_CreateMutex();
    outsync.cond = SDL_CreateCond();
    insync.lock = SDL_CreateMutex();
    insync.cond = SDL_CreateCond();

    /* Startup */
    running = true;
    thrd = SDL_CreateThread(launch, this);
    if (thrd == NULL) {
        std::string err = "Creating thread";
        err += SDL_GetError();
        throw Error(err);
    }
}

data::Point Canvas::build_point (unsigned x, unsigned y)
{
    /* Points are normalized with respect to surface size. */
    double xnorm = static_cast<double>(x) / width;
    double ynorm = 1.0 - static_cast<double>(y) / height;

    /* The normalized values are rescaled according to the ranges. */
    return data::Point(xrange.scale(xnorm), yrange.scale(ynorm));
}

int Canvas::run ()
{
    SDL_Event ev;

    update();
    while (running) {
        while (SDL_PollEvent(&ev)) {
            handle_event(ev);
        }
        check_work();
    }

    return 0;
}

void Canvas::halt ()
{
    running = false;
    SDL_CondBroadcast(outsync.cond);
    SDL_CondBroadcast(insync.cond);
}

void Canvas::handle_event (SDL_Event &ev)
{
    switch (ev.type) {
        case SDL_QUIT:
            halt();
            break;
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q:
                    halt();
                    break;
                default:
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            entered_point(ev.button.x, ev.button.y);
            break;
        default:
            break;
    }
}

void Canvas::entered_point (int x, int y)
{
    data::Point p = build_point(x, y);

    SDL_mutexP(outsync.lock);
    out.push_front(p);
    SDL_CondSignal(outsync.cond);
    SDL_mutexV(outsync.lock);
}

bool Canvas::read (data::Point &c)
{
    bool ret;

    SDL_mutexP(outsync.lock);
    while (out.empty() && running) {
        SDL_CondWait(outsync.cond, outsync.lock);
    }
    ret = running;
    if (running) {
        c = out.back();
        out.pop_back();
    }
    SDL_mutexV(outsync.lock);

    return ret;
}

void Canvas::write (data::Point const &p)
{
    SDL_mutexP(insync.lock);

    shown.push_front(p);

    SDL_CondSignal(insync.cond);
    SDL_mutexV(insync.lock);
}

void Canvas::write (std::list<data::Point> const &ps)
{
    SDL_mutexP(insync.lock);

    for (auto i = ps.begin(); i != ps.end(); i ++) {
        shown.push_front(*i ++);
    }

    SDL_CondSignal(insync.cond);
    SDL_mutexV(insync.lock);
}

unsigned Canvas::scale_x (double x)
{
    assert(0 <= x && x <= 1);
    return width * x;
}

unsigned Canvas::scale_y (double y)
{
    assert(0 <= y && y <= 1);
    return (1 - y) * height;
}

void Canvas::draw_point (data::Point const &p)
{
    std::stringstream label;

    unsigned x = scale_x(xrange.unscale(p.get_x()));
    unsigned y = scale_y(yrange.unscale(p.get_y()));

    label << p;
    aacircleColor(surface, x, y, 2, DOT_COLOR);
    stringColor(surface, x, y, label.str().c_str(), LAB_COLOR);
}

void Canvas::draw_axis ()
{
    vlineColor(surface, scale_x(xrange.unscale(0)), 0, height, AXIS_COLOR);
    hlineColor(surface, 0, width, scale_y(yrange.unscale(0)), AXIS_COLOR);
}

void Canvas::check_work ()
{
    SDL_mutexP(insync.lock);
    if (SDL_CondWaitTimeout(insync.cond, insync.lock, SLEEP) == 0) {
        update();
    }
    SDL_mutexV(insync.lock);
}

void Canvas::update ()
{
    /* NOTE: to be called only with locked input */

    draw_axis();
    for (auto i = shown.begin(); i != shown.end(); i ++) {
        draw_point(*i);
    }

    SDL_Flip(surface);
}

Canvas::~Canvas ()
{
    if (thrd) SDL_WaitThread(thrd, NULL);

    if (outsync.lock) SDL_DestroyMutex(outsync.lock);
    if (outsync.cond) SDL_DestroyCond(outsync.cond);
    if (insync.lock) SDL_DestroyMutex(insync.lock);
    if (insync.cond) SDL_DestroyCond(insync.cond);
    if (surface) SDL_FreeSurface(surface);

    SDL_Quit();
}

