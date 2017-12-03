#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <mpfr.h>
#include <cmath>

// TODO: port to non-POSIX
#include <unistd.h>

#include "real.h"
#include "complex.h"
#include "mandelbrot.h"
#include "image_writer.h"

void printHelp() {
    std::cout << "\t-p NUM\t\tSets the precision to NUM bits." << std::endl
              << "\t-t NUM\t\tSets the number of iteration threads to NUM." << std::endl
              << "\t-f FILE\t\tLoads fractal parameters from FILE." << std::endl
              << "\t-r FILE\t\tOutputs raw fractal data to FILE." << std::endl
              << "\t-o FILE\t\tOutputs PNG rendering of fractal to FILE." << std::endl
              << "\t-R\t\tRuns the program in rendering mode." << std::endl
              << "\t-I FILE\t\tLoads raw fractal data from FILE." << std::endl
              << "\t-h\t\tPrints this help message." << std::endl;
}

const double valMax = std::pow(2, sizeof(channel_t) * 8) - 1.0;

channel_t sineCurve(unsigned int k, unsigned int iterMax) {
    const double PI2 = 3.14159265357989 / 2.0;
    double frac = std::sin(PI2 * (double) k / (double) iterMax);

    return (channel_t) (valMax * frac);
}

channel_t logCurve(unsigned int k, unsigned int iterMax) {
    const double em = 1.718281828459045;
    double frac = std::log(1.0 + em * (double) k / (double) iterMax);

    return (channel_t) (valMax * frac);
}

channel_t sqrtCurve(unsigned int k, unsigned int iterMax) {
    double frac = std::sqrt((double) k / (double) iterMax);

    return (channel_t) (valMax * frac);
}

Image genImage(const Mandelbrot &m) {
    Image im(m.getWidth(), m.getHeight());

    Pixel *palette = new Pixel[m.getIterMax() + 1];

    for(unsigned int k = 0; k < m.getIterMax(); ++k) {
        palette[k].data[0] = logCurve(k * k, m.getIterMax() * m.getIterMax());
        palette[k].data[1] = sineCurve(k, m.getIterMax());//logCurve(k, m.getIterMax());
        palette[k].data[2] = sqrtCurve(k, m.getIterMax()) / 2;//sineCurve(k, m.getIterMax()) / 3;
    }

    palette[m.getIterMax()].data[0] = 255;
    palette[m.getIterMax()].data[1] = 255;
    palette[m.getIterMax()].data[2] = 255;

    for(unsigned int j = 0; j < m.getHeight(); ++j) {
        for(unsigned int i = 0; i < m.getWidth(); ++i) {
            im.pixels[j * m.getWidth() + i] = palette[m.getDataAt(i,j).inSet ? m.getIterMax() : m.getDataAt(i,j).iter];
        }
    }

    delete[] palette;

    return im;
}

int main(int argc, char **argv) {
    std::string params, rawOut, rawIn, pngOut;
    bool rendering = false;
    mpfr_prec_t precision = 64;
    unsigned int threads = 1;
    int c;

    while((c = getopt(argc, argv, "f:r:o:p:t:I:hR")) != -1) {
        switch(c) {
            case 'f':
                params = std::string(optarg);
                break;
            case 'r':
                rawOut = std::string(optarg);
                break;
            case 'o':
                pngOut = std::string(optarg);
                break;
            case 'R':
                rendering = true;
                break;
            case 'I':
                rawIn = std::string(optarg);
                break;
            case 'p':
                precision = std::stoul(optarg);
                break;
            case 't':
                threads = std::stoul(optarg);
                break;
            case 'h':
                printHelp();
                return 0;
            default:
                return -1;
        }
    }
    
    std::cout << "multiple-precision fractal generator v0.1 [altanh@uw.edu]" << std::endl;
    std::cout << "\tprecision (bits): " << precision << std::endl;
    std::cout << "\tthreads: " << threads << std::endl;

    mpfr_set_default_prec(precision);

    Mandelbrot mandel;

    if(!rendering) {
        if(!params.empty()) {
            std::cout << "-- loading fractal params from \"" + params + "\"" << std::endl;
            std::ifstream file(params.c_str());
            if(!file.is_open()) {
                std::cerr << "error: could not open file!" << std::endl;

                return -1;
            }

            mandel.readIStream(file);

            file.close();
        } else {
            mandel.readIStream(std::cin);
        }

        mandel.run(threads);
    } else {
        if(!rawIn.empty()) {
            std::cout << "-- reading raw from \"" << rawIn << "\"" << std::endl;
            if(!mandel.readRaw(rawIn)) {
                std::cerr << "error: read failed!" << std::endl;

                return -1;
            }

            mandel.print();
        } else {
            std::cerr << "error: raw input must be specified for rendering mode!" << std::endl;

            return -1;
        }
    }

    if(!rawOut.empty()) {
        std::cout << "-- writing raw to \"" << rawOut << "\"" << std::endl;
        if(!mandel.writeRaw(rawOut)) {
            std::cerr << "error: write failed!" << std::endl;

            return -1;
        }
    }

    if(!pngOut.empty()) {
        Image outputImage = genImage(mandel);

        std::cout << "-- writing PNG to \"" << pngOut << "\"" << std::endl;
        if(!ImageWriter::writePNG(outputImage, pngOut)) {
            std::cerr << "error: write failed!" << std::endl;

            return -1;
        }
    }

    return 0;
}