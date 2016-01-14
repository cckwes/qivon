#include <iostream>
#include <pngio.h>
#include <geometry.h>

#include "timing.h"
#include "test_geometry.h"

bool test_resize_image(qivon::Image<unsigned char> &_src, size_t _width, size_t _height) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> resized_img;

  set_start_now();

  qivon::resizeImage(_src, resized_img, _width, _height);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() << " ms\n";

  if (resized_img.isEmpty())
    return false;
  else
    return qivon::writePngFile("resize.png", resized_img);
}

bool test_crop_image(qivon::Image<unsigned char> &_src,
                     size_t _start_x, size_t _start_y, size_t _end_x, size_t _end_y) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> cropped_img;

  set_start_now();

  qivon::cropImage(_src, cropped_img, _start_x, _start_y, _end_x, _end_y);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() << " ms\n";

  if (cropped_img.isEmpty())
    return false;
  else
    return qivon::writePngFile("crop.png", cropped_img);
}