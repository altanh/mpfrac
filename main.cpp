#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <mpfr.h>

#include "real.h"
#include "complex.h"
#include "mandelbrot.h"

#define PRECISION 1024

struct IterInfo {
    unsigned int iter;
    unsigned int count;
};

int main(int argc, char **argv) {
    mpfr_prec_t precision;
    
    std::cout << "multiple-precision fractal generator v0.1 [altanh@uw.edu]" << std::endl;
    std::cout << "\tprecision (bits): ";
    std::cin >> precision;

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

    mandel.run();

    std::vector<IterInfo> iterCounts;

    for(unsigned int i = 0; i < mandel.getIterMax(); ++i)
        iterCounts.push_back({i, 0});

    for(unsigned int y = 0; y < mandel.getHeight(); ++y) {
        for(unsigned int x = 0; x < mandel.getWidth(); ++x) {
            iterCounts[mandel.getDataAt(x,y).iter].count++;
        }
    }

    std::sort(iterCounts.begin(), iterCounts.end(), [](const IterInfo &a, const IterInfo &b){return a.count > b.count;});

    for(unsigned int y = 0; y < mandel.getHeight(); ++y) {
        for(unsigned int x = 0; x < mandel.getWidth(); ++x) {
            if(mandel.getDataAt(x,y).inSet) {
                std::cout << " ";
            } else {
                unsigned int iter = mandel.getDataAt(x,y).iter;
                int iterIndex = std::distance(iterCounts.begin(), std::find_if(iterCounts.begin(), iterCounts.end(), [&iter](const IterInfo &it){return it.iter == iter;}));
            
                switch(iterIndex) {
                    case 0:
                        std::cout << "#";
                        break;
                    case 1:
                        std::cout << "$";
                        break;
                    case 2:
                        std::cout << "%";
                        break;
                    case 3:
                        std::cout << "^";
                        break;
                    case 4:
                        std::cout << "*";
                        break;
                    case 5:
                        std::cout << ".";
                        break;
                    default:
                        std::cout << "`";
                        break;
                }
            }
        }

        std::cout << std::endl;
    }

    return 0;
}