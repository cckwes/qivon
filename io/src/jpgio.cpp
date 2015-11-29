#include <stdio.h>
extern "C" {
#include <jpeglib.h>
}

#include <setjmp.h>
#include <iostream>

#include "jpgio.h"

namespace qivon {

//error handler struture that is REQUIRED
struct my_error_mgr {
  jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

//routine for replacing the standard error_exit method, REQUIRED
METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

Image<unsigned char> readJpgFile(const std::string &filename) {
  jpeg_decompress_struct cinfo;

  my_error_mgr jerr;

  FILE *fp;
  JSAMPARRAY buffer;
  int row_bytes;

    fp = fopen(filename.c_str(), "rb");

  if (!fp) {
    std::cerr << "Unable to open file " << filename << std::endl;
    return Image<unsigned char>();
  }

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  if (setjmp(jerr.setjmp_buffer)) {
    std::cerr << "IO error\n";
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);
    return Image<unsigned char>();
  }

  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, fp);

  (void) jpeg_read_header(&cinfo, true);

  (void) jpeg_start_decompress(&cinfo);

  if (cinfo.output_components != 3)
  {
    std::cerr << "Can only handle 3 channel image at the moment. "
        << "Current image is " << cinfo.output_components << " channel(s)\n";
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);
    return Image<unsigned char>();
  }

  row_bytes = cinfo.output_width * cinfo.output_components;
  size_t width = cinfo.output_width;
  size_t height = cinfo.output_height;

  buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_bytes, 1);

  //allocate the image data buffer
  unsigned char *image_data = (unsigned char *) malloc(row_bytes * height);

  size_t image_size = width * height;

  int y = 0;
  while (cinfo.output_scanline < height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);

    for (int x = 0; x < width; ++x) {
      image_data[y * width + x] = buffer[0][3 * x];
      image_data[image_size + y * width + x] = buffer[0][3 * x + 1];
      image_data[2 * image_size + y * width + x] = buffer[0][3 * x + 2];
    }
    ++y;
  }

  (void) jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);

  fclose(fp);

  return Image<unsigned char>(width, height, 3, Type_RGB, image_data);
}

bool writeJpgFile(const std::string &filename,
                  Image<unsigned char> &image,
                  unsigned int quality) {
  if (image.isEmpty()) {
    std::cerr << "Empty image, not exporting\n";
    return false;
  }

  if (image.channels() != 3 && image.channels() != 1) {
    std::cerr << "Can only output image with 3 channels\n";
    return false;
  }

  if (quality == 0 || quality > 100) {
    std::cerr << "Invalid quality\n";
    return false;
  }

  struct jpeg_compress_struct cinfo;

  struct jpeg_error_mgr jerr;
  JSAMPROW  row_pointer[1];
  int row_stride;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  FILE *fp = fopen(filename.c_str(), "wb");

  if (!fp) {
    std::cerr << "Error open " << filename << " for jpg exporting\n";
    return false;
  }

  jpeg_stdio_dest(&cinfo, fp);

  //entering image info into cinfo structure
  cinfo.image_width = image.width();
  cinfo.image_height = image.height();
  if (image.channels() == 3) {
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    row_stride = image.width() * 3;

  } else if (image.channels() == 1) {
    cinfo.input_components = 1;
    cinfo.in_color_space = JCS_GRAYSCALE;

    row_stride = image.width();
  }

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

  unsigned char *buffer = (unsigned char *) malloc(row_stride * sizeof(unsigned char));
  unsigned char *image_buffer = image.data();
  size_t width = image.width();
  size_t height = image.height();
  size_t image_size = width * height;

  size_t current_height = 0;
  while (cinfo.next_scanline < cinfo.image_height) {
    for (int i = 0; i < image.width(); ++i) {
      if (image.channels() == 3) {
        //green buffer at same location for BGR and RGB
        buffer[i * 3 + 1] = image_buffer[current_height * width + i + image_size];

        if (image.color() == Type_RGB) {
          buffer[i * 3] = image_buffer[current_height * width + i];
          buffer[i * 3 + 2] = image_buffer[current_height * width + i + 2 * image_size];
        } else if (image.color() == Type_BGR) {
          buffer[i * 3] = image_buffer[current_height * width + i + 2 * image_size];
          buffer[i * 3 + 2] = image_buffer[current_height * width + i];
        }
      } else {
        //single channel grayscale image
        buffer[i] = image_buffer[current_height * width + i];
      }
    }
    row_pointer[0] = buffer;
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    ++current_height;
  }

  jpeg_finish_compress(&cinfo);
  fclose(fp);

  jpeg_destroy_compress(&cinfo);

  return true;
}

}       //namespace qivon