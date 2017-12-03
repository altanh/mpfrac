#include "image_writer.h"

Image::Image() : pixels(nullptr) {

}

Image::Image(unsigned int w, unsigned int h) : width(w), height(h) {
    pixels = new Pixel[w * h];
}

Image::Image(unsigned int w, unsigned int h, const Pixel *p) : width(w), height(h) {
    pixels = new Pixel[w * h];
    for(unsigned int j = 0; j < h; ++j)
        for(unsigned int i = 0; i < w; ++i)
            pixels[j * w + i] = p[j * w + i];
}

Image::~Image() {
    if(pixels)
        delete[] pixels;
}

png_bytepp ImageWriter::_getRows(const Image &im) {
    png_bytepp rows = new png_bytep[im.height];

    for(unsigned int j = 0; j < im.height; ++j)
        rows[j] = (png_bytep) &im.pixels[j * im.width];

    return rows;
}

void ImageWriter::_delRows(png_bytepp rows) {
    if(rows)
        delete[] rows;
}

bool ImageWriter::writePNG(const Image &im, std::string filename) {
    FILE *fp = fopen(filename.c_str(), "wb");
    if(!fp) return false;

    png_structp writeStruct = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, (png_voidp) NULL, NULL, NULL);

    if(!writeStruct) return false;

    png_infop infoStruct = png_create_info_struct(writeStruct);
    
    if(!infoStruct) {
        png_destroy_write_struct(&writeStruct, (png_infopp) NULL);
        
        return false;
    }

    if(setjmp(png_jmpbuf(writeStruct))) {
        png_destroy_write_struct(&writeStruct, &infoStruct);
        fclose(fp);

        return false;
    }

    png_init_io(writeStruct, fp);

    png_set_IHDR(writeStruct, infoStruct, im.width, im.height, sizeof(channel_t) * 8,
            PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

    png_bytepp rows = _getRows(im);
    png_set_rows(writeStruct, infoStruct, rows);

    png_write_png(writeStruct, infoStruct, PNG_TRANSFORM_IDENTITY, NULL);

    png_write_end(writeStruct, NULL);

    png_destroy_write_struct(&writeStruct, &infoStruct);

    _delRows(rows);

    fclose(fp);

    return true;
}