#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <mpfr.h>
#include <cmath>

#include "real.h"
#include "complex.h"
#include "mandelbrot.h"
#include "image_writer.h"

struct IterInfo {
    unsigned int iter;
    unsigned int count;
};

int main(int argc, char **argv) {
    mpfr_prec_t precision;
    unsigned int threads;
    
    std::cout << "multiple-precision fractal generator v0.1 [altanh@uw.edu]" << std::endl;
    std::cout << "\tprecision (bits): ";
    std::cin >> precision;
    std::cout << "\tthreads: ";
    std::cin >> threads;

    mpfr_set_default_prec(precision);

    Mandelbrot mandel;

    if(argc > 1) {
        std::cout << "-- loading fractal params from \"" + std::string(argv[1]) + "\"" << std::endl;
        std::ifstream file(argv[1]);
        if(!file.is_open()) {
            std::cerr << "could not open file!" << std::endl;
            return -1;
        }

        mandel = Mandelbrot::fromIStream(file);
    } else {
        mandel = Mandelbrot::fromIStream(std::cin);
    }

    mandel.run(threads);

    /*std::vector<IterInfo> iterCounts;

    for(unsigned int i = 0; i < mandel.getIterMax(); ++i)
        iterCounts.push_back({i, 0});

    for(unsigned int y = 0; y < mandel.getHeight(); ++y) {
        for(unsigned int x = 0; x < mandel.getWidth(); ++x) {
            iterCounts[mandel.getDataAt(x,y).iter].count++;
        }
    }

    std::sort(iterCounts.begin(), iterCounts.end(), [](const IterInfo &a, const IterInfo &b){return a.count > b.count;});

    std::vector<std::string> chars = {
        "#", "+", "*", "~", "\"", "\'", ":", "`", ".", ","
    };

    for(unsigned int y = 0; y < mandel.getHeight(); ++y) {
        for(unsigned int x = 0; x < mandel.getWidth(); ++x) {
            if(mandel.getDataAt(x,y).inSet) {
                std::cout << " ";
            } else {
                unsigned int iter = mandel.getDataAt(x,y).iter;
                int iterIndex = std::distance(iterCounts.begin(), std::find_if(iterCounts.begin(), iterCounts.end(), [&iter](const IterInfo &it){return it.iter == iter;}));
            
                if(iterIndex < chars.size())
                    std::cout << chars[iterIndex];
                else {
                    unsigned int closestDiff = mandel.getIterMax();
                    unsigned int closestIndex = 0;
                    for(unsigned int i = 0; i < chars.size(); ++i) {
                        unsigned int diff = std::abs((int)iterCounts[i].iter - (int)iter);
                        if(diff < closestDiff) {
                            closestDiff = diff;
                            closestIndex = i;
                        }
                    }

                    std::cout << chars[closestIndex];
                }
            }
        }

        std::cout << std::endl;
    }*/

    Image mandelOut(mandel.getWidth(), mandel.getHeight());

    for(unsigned int j = 0; j < mandel.getHeight(); ++j) {
        for(unsigned int i = 0; i < mandel.getWidth(); ++i) {
            mandelOut.pixels[j * mandel.getWidth() + i].data[0] = mandel.getDataAt(i,j).inSet ? 255 : 0;
            mandelOut.pixels[j * mandel.getWidth() + i].data[1] = 0;
            mandelOut.pixels[j * mandel.getWidth() + i].data[2] = 0;
        }
    }

    std::cout << "writing to \"mandel.png\"" << std::endl;
    if(!ImageWriter::writePNG(mandelOut, "mandel.png")) {
        std::cerr << "write failed!" << std::endl;

        return -1;
    }

    return 0;
}