#include <iostream>
#include <png.h>

#include "pngio.h"

namespace qivon {

Image<unsigned char> readPngFile(const std::string &filename) {
  char header[8];

  //open file
  FILE *fp = fopen(filename.c_str(), "rb");

  //test if open file successful
  if (!fp) {
    std::cerr << "File " << filename << " cannot be open for reading." << std::endl;
    return Image<unsigned char>();
  }

  //check if file type is PNG
  fread(header, 1, 8, fp);
  if (png_sig_cmp((unsigned char *) header, 0, 8)) {
    std::cerr << "File " << filename << " is not a recognized PNG file." << std::endl;
    return Image<unsigned char>();
  }

  //create png read structure
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    std::cerr << filename << " png_create_read_struct failed." << std::endl;
    return Image<unsigned char>();
  }

  //create png info structure
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    std::cerr << filename << " png_create_info_struct failed." << std::endl;
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return Image<unsigned char>();
  }

  //initialize io
  if (setjmp(png_jmpbuf(png_ptr))) {
    std::cerr << filename << " error during init io." << std::endl;
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return Image<unsigned char>();
  }

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  //read information from png file
  png_read_info(png_ptr, info_ptr);

  size_t width = png_get_image_width(png_ptr, info_ptr);
  size_t height = png_get_image_height(png_ptr, info_ptr);
  png_byte color_type = png_get_color_type(png_ptr, info_ptr);
  png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  int number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  //start reading the content
  if (setjmp(png_jmpbuf(png_ptr))) {
    std::cerr << filename << " error during read file." << std::endl;
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return Image<unsigned char>();
  }

  //can only 8bit at the moment
  if (bit_depth != 8) {
    std::cerr << filename << " bit depth is not 8" << std::endl;
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return Image<unsigned char>();
  }

  //can only read Grayscale, RGB and RGBA image at the moment
  if (color_type != PNG_COLOR_TYPE_RGB &&
      color_type != PNG_COLOR_TYPE_RGBA &&
      color_type != PNG_COLOR_TYPE_GRAY) {
    std::cerr << filename << " color type is not supported (" << color_type << ")" << std::endl;
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return Image<unsigned char>();
  }

  //declare and allocate buffer for file reading
  png_bytep *row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);

  size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);

  for (int y = 0; y < height; ++y) {
    row_pointers[y] = (png_byte *) malloc(row_bytes);
  }

  //read image content into the buffer
  png_read_image(png_ptr, row_pointers);
  fclose(fp);

  size_t image_size = width * height;

  //copy buffer into image data
  unsigned char *image_data = (unsigned char *) malloc(row_bytes * height);
  for (int y = 0; y < height; ++y) {
    const png_byte *row = row_pointers[y];

    for (int x = 0; x < width; ++x) {
      if (color_type == PNG_COLOR_TYPE_GRAY) {
        image_data[y * width + x] = row[x];
      } else if (color_type == PNG_COLOR_TYPE_RGB) {
        image_data[y * width + x] = row[3 * x];
        image_data[image_size + y * width + x] = row[3 * x + 1];
        image_data[2 * image_size + y * width + x] = row[3 * x + 2];
      } else if (color_type == PNG_COLOR_TYPE_RGBA) {
        image_data[y * width + x] = row[4 * x];
        image_data[image_size + y * width + x] = row[4 * x + 1];
        image_data[2 * image_size + y * width + x] = row[4 * x + 2];
        image_data[3 * image_size + y * width + x] = row[4 * x + 3];
      }
    }
  }

  //cleanup
  for (int y = 0; y < height; ++y) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  png_destroy_read_struct(&png_ptr, NULL, NULL);

  if (color_type == PNG_COLOR_TYPE_GRAY)
    return Image<unsigned char>(width, height, 1, Type_Grayscale, image_data);
  if (color_type == PNG_COLOR_TYPE_RGB)
    return Image<unsigned char>(width, height, 3, Type_RGB, image_data);
  else if (color_type == PNG_COLOR_TYPE_RGBA)
    return Image<unsigned char>(width, height, 4, Type_RGBA, image_data);
  else
    return Image<unsigned char>();
}

bool writePngFile(const std::string &filename, Image<unsigned char> &image) {
  //image empty check
  if (image.isEmpty()) {
    std::cerr << "Empty image, not exporting\n";
    return false;
  }

  //image channels check (can only output 1, 3 or 4 channels image)
  if (image.channels() != 3
      && image.channels() != 4
      && image.channels() != 1) {
    std::cerr << "Can only output image with 1, 3 or 4 channels\n";
    return false;
  }

  //color type checking
  //can only output RGB, BGR, RGBA, Grayscale
  if (image.color() != qivon::Type_Grayscale && image.color() != qivon::Type_BGR
      && image.color() != qivon::Type_RGB && image.color() != qivon::Type_RGBA) {
    std::cerr << "Unable to output image with this color type\n";
    return false;
  }

  FILE *fp = fopen(filename.c_str(), "wb");

  if (!fp) {
    std::cerr << "Error open " << filename << " for png exporting" << std::endl;
    return false;
  }

  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    std::cerr << "png_create_write_struct failed\n";
    return false;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    std::cerr << "png_create_info_struct failed\n";
    png_destroy_write_struct(&png_ptr, NULL);
    return false;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    std::cerr << "Io init error when exporting png\n";
    png_destroy_write_struct(&png_ptr, NULL);
    return false;
  }

  png_init_io(png_ptr, fp);

  if (setjmp(png_jmpbuf(png_ptr))) {
    std::cerr << "Error when writing header\n";
    png_destroy_write_struct(&png_ptr, NULL);
    return false;
  }

  if (image.channels() == 1) {
    png_set_IHDR(png_ptr, info_ptr,
                 image.width(), image.height(),
                 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
  }
  else if (image.channels() == 3) {
    png_set_IHDR(png_ptr, info_ptr,
                 image.width(), image.height(),
                 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
  }
  else if (image.channels() == 4) {
    png_set_IHDR(png_ptr, info_ptr,
                 image.width(), image.height(),
                 8, PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
  }

  png_write_info(png_ptr, info_ptr);

  if (setjmp(png_jmpbuf(png_ptr))) {
    std::cerr << "Error when writing bytes\n";
    png_destroy_write_struct(&png_ptr, NULL);
    return false;
  }

  unsigned char *image_data = image.data();

  size_t width = image.width();
  size_t height = image.height();

  png_bytep *row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);

  size_t row_bytes = image.channels() * width;

  size_t image_size = width * height;

  for (int y = 0; y < height; ++y) {
    row_pointers[y] = (png_byte *) malloc(row_bytes);
    png_byte *row = row_pointers[y];

    for (int x = 0; x < width; ++x) {
      if (image.channels() == 1) {
        row[x] = image_data[y * width + x];
      } else if (image.channels() == 3) {
        row[3 * x] = image_data[y * width + x];
        row[3 * x + 1] = image_data[image_size + y * width + x];
        row[3 * x + 2] = image_data[2 * image_size + y * width + x];
      } else if (image.channels() == 4) {
        row[4 * x] = image_data[y * width + x];
        row[4 * x + 1] = image_data[image_size + y * width + x];
        row[4 * x + 2] = image_data[2 * image_size + y * width + x];
        row[4 * x + 3] = image_data[3 * image_size + y * width + x];
      }
    }
  }

  png_write_image(png_ptr, row_pointers);

  if (setjmp(png_jmpbuf(png_ptr))) {
    std::cerr << "Error during end of write\n";
    png_destroy_write_struct(&png_ptr, NULL);
    return false;
  }

  png_write_end(png_ptr, NULL);

  //cleanup
  for (int y = 0; y < height; ++y)
    free(row_pointers[y]);
  free(row_pointers);

  png_destroy_write_struct(&png_ptr, NULL);

  return true;
}
}       //namespace qivon