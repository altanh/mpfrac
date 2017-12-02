#include <iostream>
#include <mpfr.h>

#include "real.h"
#include "complex.h"
#include "mandelbrot.h"

#define PRECISION 1024

int main(int argc, char **argv) {
    mpfr_set_default_prec(PRECISION);

    int width = 160;
    int height = 80;

    Mandelbrot mandel(80, 40, {{Real("-2"), Real("2")}, {Real("4"), Real("4")}}, 128);

    mandel.run();

    for(unsigned int y = 0; y < mandel.getHeight(); ++y) {
        for(unsigned int x = 0; x < mandel.getWidth(); ++x) {
            std::cout << (mandel.getDataAt(x,y).inSet ? "*" : " ");
        }

        std::cout << std::endl;
    }

    return 0;
}