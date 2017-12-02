#ifndef IMAGE_WRITER_H
#define IMAGE_WRITER_H

#include <iostream>
#include <cstdio>
#include <string>
#include <png.h>

typedef unsigned char channel_t;

struct Pixel {
    channel_t data[3];
};

struct Image {
    unsigned int width;
    unsigned int height;
    Pixel *pixels;

    Image();
    Image(unsigned int w, unsigned int h);
    Image(unsigned int w, unsigned int h, const Pixel *p);

    ~Image();
};

class ImageWriter {
    static png_bytepp _getRows(const Image &im);
    static void _delRows(png_bytepp rows);
public:
    static bool writePNG(const Image &im, std::string filename);
};

#endif