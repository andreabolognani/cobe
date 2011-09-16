#include <unistd.h>
#include <iostream>
#include <iomanip>

#include "canvas.hpp"

int main ()
{
    Range x(-10, 10);
    Range y(-5, 5);

	Canvas c(640, 480, x, y);
    data::Point point;

    while (c.read(point)) {
        std::cout << " x=" << std::setw(3) << point.get_x()
                  << " y=" << std::setw(3) << point.get_y()
                  << std::endl;
        c.write(point);
    }
}
