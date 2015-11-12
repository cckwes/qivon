#include <iostream>

#include "pngio.h"

using namespace qivon;

Image<unsigned char> readPngFile(const std::string &filename)
{
    char header[8];

    FILE *fp = fopen(filename.c_str(), "rb");
    if (!fp)
    {
        std::cerr << "File " << filename << " cannot be open for reading." << std::endl;
        return Image<unsigned  char>();
    }
    fread(header, 1, 8, fp);
    if (png_sig_cmp((unsigned char*)header, 0, 8))
    {
        std::cerr << "File " << filename << " is not a recognized PNG file." << std::endl;
        return Image<unsigned char>();
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        std::cerr << filename << " png_create_read_struct failed." << std::endl;
        return Image<unsigned char>();
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        std::cerr << filename << " png_create_info_struct failed." << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return Image<unsigned char>();
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        std::cerr << filename << " error during init io." << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return Image<unsigned char>();
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    int number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        std::cerr << filename << " error during read file." << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return Image<unsigned char>();
    }

    //can only 8bit at the moment
    if (bit_depth != 8)
    {
        std::cerr << filename << " bit depth is not 8" << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return Image<unsigned char>();
    }

    if (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGBA)
    {
        std::cerr << filename << " color type is not supported (" << color_type << ")" << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return Image<unsigned char>();
    }

    png_bytep *row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    for (int y = 0; y < height; ++y)
    {
        row_pointers[y] = (png_byte*) malloc(row_bytes);
    }

    png_read_image(png_ptr, row_pointers);
    fclose(fp);

    unsigned char *image_data = (unsigned char*) malloc(row_bytes * height);
    for (int y = 0; y < height; ++y)
    {
        const png_byte *row = row_pointers[y];

        for (int x = 0; x < width; ++x)
        {
            if (color_type == PNG_COLOR_TYPE_RGB)
            {
                image_data[y * row_bytes + 3 * x] = row[3 * x];
                image_data[y * row_bytes + 3 * x + 1] = row[3 * x + 1];
                image_data[y * row_bytes + 3 * x + 2] = row[3 * x + 2];
            }
            else if (color_type == PNG_COLOR_TYPE_RGBA)
            {
                image_data[y * row_bytes + 4 * x] = row[4 * x];
                image_data[y * row_bytes + 4 * x + 1] = row[4 * x + 1];
                image_data[y * row_bytes + 4 * x + 2] = row[4 * x + 2];
                image_data[y * row_bytes + 4 * x + 3] = row[4 * x + 3];
            }
        }
    }

    //cleanup
    for (int y = 0; y < height; ++y)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return Image<unsigned char>();
}
