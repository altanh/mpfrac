#include <iostream>
#include <mpfr.h>

#include "real.h"
#include "complex.h"

#define PRECISION 1024
#define ITER_MAX 512

bool inMandelbrot(Complex c) {
    Complex z(Real::ZERO, Real::ZERO);

    const Real FOUR("4");

    for(int i = 0; i < ITER_MAX; ++i) {
        z = z * z + c;

        Real magSquared = z.re * z.re + z.im * z.im;

        if(magSquared > FOUR)
            return false;
    }

    return true;
}

int main(int argc, char **argv) {
    mpfr_set_default_prec(PRECISION);

    int width = 160;
    int height = 80;

    Complex topLeft(Real("-2"), Real("2"));

    for(int j = 0; j < height; ++j) {
        for(int i = 0; i < width; ++i) {
            Complex c = topLeft + Complex(Real("4") / Real((double) (width)) * Real((double) i), 
                                          Real("-4") / Real((double (height))) * Real((double) j));

            bool inSet = inMandelbrot(c);

            std::cout << (inSet ? "*" : " ");
        }

        std::cout << std::endl;
    }

    return 0;
}