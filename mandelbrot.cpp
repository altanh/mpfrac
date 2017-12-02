#include "mandelbrot.h"

Mandelbrot::Mandelbrot() {
    _data = nullptr;
}

Mandelbrot::Mandelbrot(unsigned int width, unsigned int height,
                       Viewport view, unsigned int iterMax)
        : _width(width), _height(height), _viewport(view), _iterMax(iterMax) {

}

Mandelbrot Mandelbrot::fromIStream(std::istream &is) {
    const bool isCin = &is == &std::cin;

    unsigned int width, height, iterMax;
    std::string viewRe, viewIm, viewWidth, viewHeight;

    std::cout << "\twidth: ";
    is >> width;
    if(!isCin) std::cout << width << std::endl;

    std::cout << "\theight: ";
    is >> height;
    if(!isCin) std::cout << height << std::endl;

    std::cout << "\titerMax: ";
    is >> iterMax;
    if(!isCin) std::cout << iterMax << std::endl;

    std::cout << "\tviewport: " << std::endl << "\t\tre: ";
    is >> viewRe;
    if(!isCin) std::cout << viewRe << std::endl;

    std::cout << "\t\tim: ";
    is >> viewIm;
    if(!isCin) std::cout << viewIm << std::endl;

    std::cout << "\t\twidth: ";
    is >> viewWidth;
    if(!isCin) std::cout << viewWidth << std::endl;

    std::cout << "\t\theight: ";
    is >> viewHeight;
    if(!isCin) std::cout << viewHeight << std::endl;

    return Mandelbrot(width, height, 
            {{Real(viewRe), Real(viewIm)}, {Real(viewWidth), Real(viewHeight)}}, iterMax);
}

Mandelbrot::~Mandelbrot() {
    if(_data)
        delete[] _data;
}

void Mandelbrot::_runChunk(Mandelbrot *m, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    const Real FOUR("4");
    const Real DELTA_RE = m->_viewport.size.re / Real(m->_width);
    const Real DELTA_IM = m->_viewport.size.im / Real(m->_height);

    for(unsigned int j = y; j < height + y; ++j) {
        for(unsigned int i = x; i < width + x; ++i) {
            const unsigned int index = j * m->_width + i;
            const Complex c = m->_viewport.topLeft + Complex(DELTA_RE * Real(i),
                                                             DELTA_IM.neg() * Real(j));

            Complex z(Real::ZERO, Real::ZERO);

            m->_data[index].inSet = true;

            for(unsigned int iter = 0; iter < m->_iterMax; ++iter) {
                z = z * z + c;

                if(z.re * z.re + z.im * z.im > FOUR) {
                    m->_data[index].iter = iter;
                    m->_data[index].inSet = false;
                }
            }
        }
    }
}

void Mandelbrot::run(unsigned int threadCount) {
    if(_data)
        delete[] _data;

    _data = new MandelbrotData[_width * _height];

    unsigned int chunkHeight = _height / threadCount;
    unsigned int chunkRemainder = _height % threadCount;

    std::vector<std::thread> threads;

    std::cout << "-- running with " << threadCount << " threads" << std::endl;
    std::cout << "\tchunkHeight = " << chunkHeight << std::endl
              << "\tchunkRemainder = " << chunkRemainder << std::endl;

    for(unsigned int i = 0; i < threadCount; ++i) {
        if(i == threadCount - 1) {
            threads.emplace_back(_runChunk, this, 0, i * chunkHeight, _width, chunkHeight + chunkRemainder);
        } else {
            threads.emplace_back(_runChunk, this, 0, i * chunkHeight, _width, chunkHeight);
        }
    }

    for(unsigned int i = 0; i < threadCount; ++i) {
        threads[i].join();
    }
}

void Mandelbrot::setViewport(Viewport view) {
    _viewport = view;
}

const Viewport& Mandelbrot::getViewport() const {
    return _viewport;
}

void Mandelbrot::setWidth(unsigned int width) {
    _width = width;
}

unsigned int Mandelbrot::getWidth() const {
    return _width;
}

void Mandelbrot::setHeight(unsigned int height) {
    _height = height;
}

unsigned int Mandelbrot::getHeight() const {
    return _height;
}

void Mandelbrot::setIterMax(unsigned int iterMax) {
    _iterMax = iterMax;
}

unsigned int Mandelbrot::getIterMax() const {
    return _iterMax;
}

const MandelbrotData* Mandelbrot::getData() const {
    return _data;
}

const MandelbrotData& Mandelbrot::getDataAt(unsigned int x, unsigned int y) const {
    return _data[y * _width + x];
}