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
  int width = cinfo.output_width;
  int height = cinfo.output_height;

  buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_bytes, 1);

  //allocate the image data buffer
  unsigned char *image_data = (unsigned char *) malloc(row_bytes * height);

  int y = 0;
  while (cinfo.output_scanline < height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);

    for (int x = 0; x < width; ++x) {
      image_data[y * row_bytes + 3 * x] = buffer[0][3 * x];
      image_data[y * row_bytes + 3 * x + 1] = buffer[0][3 * x + 1];
      image_data[y * row_bytes + 3 * x + 2] = buffer[0][3 * x + 2];
    }
    ++y;
  }

  (void) jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);

  fclose(fp);

  return Image<unsigned char>(width, height, 3, image_data);
}

bool writeJpgFile(const std::string &filename, Image<unsigned char> &image) {
  if (image.isEmpty()) {
    std::cerr << "Empty image, not exporting\n";
    return false;
  }

  if (image.channels() != 3) {
    std::cerr << "Can only output image with 3 channels\n";
    return false;
  }

  FILE *fp = fopen(filename.c_str(), "wb");

  if (!fp) {
    std::cerr << "Error open " << filename << " for jpg exporting\n";
    return false;
  }

  return true;
}

}       //namespace qivon