#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <iostream>
#include <vector>
#include <thread>

#include "complex.h"

struct Viewport {
    Complex topLeft;
    Complex size;
};

struct MandelbrotData {
    unsigned int iter;
    bool inSet;
};

class Mandelbrot {
    MandelbrotData *_data;

    unsigned int _width;
    unsigned int _height;

    unsigned int _iterMax;

    Viewport _viewport;

    static void _runChunk(Mandelbrot *m, unsigned int x, unsigned int y, unsigned int w, unsigned int h);

public:
    Mandelbrot();
    Mandelbrot(unsigned int width, unsigned int height, 
               Viewport view, unsigned int iterMax);

    static Mandelbrot fromIStream(std::istream &is);

    ~Mandelbrot();

    void run(unsigned int threadCount = 1);

    void setViewport(Viewport view);
    const Viewport& getViewport() const;

    void setWidth(unsigned int width);
    unsigned int getWidth() const;

    void setHeight(unsigned int height);
    unsigned int getHeight() const;

    void setIterMax(unsigned int iterMax);
    unsigned int getIterMax() const;

    const MandelbrotData* getData() const;
    const MandelbrotData& getDataAt(unsigned int x, unsigned int y) const;
};

#endif