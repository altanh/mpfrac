#include "mandelbrot.h"

Mandelbrot::Mandelbrot() {
    _data = nullptr;
}

Mandelbrot::Mandelbrot(unsigned int width, unsigned int height,
                       Viewport view, unsigned int iterMax)
        : _width(width), _height(height), _viewport(view), _iterMax(iterMax) {

}

void Mandelbrot::readIStream(std::istream &is) {
    const bool isCin = &is == &std::cin;

    std::string viewRe, viewIm, viewWidth, viewHeight;

    std::cout << "\twidth = ";
    is >> _width;
    if(!isCin) std::cout << _width << std::endl;

    std::cout << "\theight = ";
    is >> _height;
    if(!isCin) std::cout << _height << std::endl;

    std::cout << "\titerMax = ";
    is >> _iterMax;
    if(!isCin) std::cout << _iterMax << std::endl;

    std::cout << "\tviewport: " << std::endl << "\t\tre = ";
    is >> viewRe;
    if(!isCin) std::cout << viewRe << std::endl;

    std::cout << "\t\tim = ";
    is >> viewIm;
    if(!isCin) std::cout << viewIm << std::endl;

    std::cout << "\t\twidth = ";
    is >> viewWidth;
    if(!isCin) std::cout << viewWidth << std::endl;

    std::cout << "\t\theight = ";
    is >> viewHeight;
    if(!isCin) std::cout << viewHeight << std::endl;

    _viewport.topLeft.re = Real(viewRe);
    _viewport.topLeft.im = Real(viewIm);
    _viewport.size.re = Real(viewWidth);
    _viewport.size.im = Real(viewHeight);
}

Mandelbrot::~Mandelbrot() {
    if(_data)
        delete[] _data;
}

bool Mandelbrot::writeRaw(std::string filename) const {
    std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    if(!file.is_open()) return false;

    file << _width << "\n";
    file << _height << "\n";
    file << _iterMax << "\n";
    file << _viewport.topLeft.re.toString(_viewport.topLeft.re.getPrecision()) << "\n";
    file << _viewport.topLeft.im.toString(_viewport.topLeft.im.getPrecision()) << "\n";
    file << _viewport.size.re.toString(_viewport.size.re.getPrecision()) << "\n";
    file << _viewport.size.im.toString(_viewport.size.im.getPrecision()) << "\n";
    file.write((char*)_data, sizeof(MandelbrotData) * _width * _height);

    file.close();

    return true;
}

bool Mandelbrot::readRaw(std::string filename) {
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    if(!file.is_open()) return false;

    std::string header[7];
    for(unsigned int i = 0; i < 7; ++i) {
        std::getline(file, header[i]);
    }

    std::stringstream(header[0]) >> _width;
    std::stringstream(header[1]) >> _height;
    std::stringstream(header[2]) >> _iterMax;
    _viewport.topLeft = Complex(Real(header[3]), Real(header[4]));
    _viewport.size = Complex(Real(header[5]), Real(header[6]));

    _data = new MandelbrotData[_width * _height];

    file.read((char*) _data, sizeof(MandelbrotData) * _width * _height);

    return true;
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

    auto start = std::chrono::high_resolution_clock::now();

    for(unsigned int i = 0; i < threadCount; ++i) {
        if(i == threadCount - 1) {
            threads.emplace_back(_runChunk, this, 0, i * chunkHeight, _width, chunkHeight + chunkRemainder);
        } else {
            threads.emplace_back(_runChunk, this, 0, i * chunkHeight, _width, chunkHeight);
        }
    }

    for(unsigned int i = 0; i < threadCount; ++i) {
        threads[i].join();
        std::cout << "-> thread " << i + 1 << " completed" << std::endl;
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "[took " << std::chrono::duration_cast<std::chrono::seconds>(finish - start).count() << " seconds]" << std::endl;
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

void Mandelbrot::print() const {
    std::cout << "\twidth = " << _width << std::endl
              << "\theight = " << _height << std::endl
              << "\titerMax = " << _iterMax << std::endl
              << "\tviewport: " << std::endl
              << "\t\tre = " << _viewport.topLeft.re.toString(_viewport.topLeft.re.getPrecision()) << std::endl
              << "\t\tim = " << _viewport.topLeft.im.toString(_viewport.topLeft.im.getPrecision()) << std::endl
              << "\t\twidth = " << _viewport.size.re.toString(_viewport.size.re.getPrecision()) << std::endl
              << "\t\theight = " << _viewport.size.im.toString(_viewport.size.im.getPrecision()) << std::endl;
}