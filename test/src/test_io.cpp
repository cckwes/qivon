#include <pngio.h>
#include <jpgio.h>
#include <iostream>

#include "test_io.h"

bool test_read_png(const std::string &_filename) {
  qivon::Image<unsigned char> png_img = qivon::readPngFile(_filename);

  return !png_img.isEmpty();
}

bool test_read_jpg(const std::string &_filename) {
  qivon::Image<unsigned char> jpg_img = qivon::readJpgFile(_filename);

  return !jpg_img.isEmpty();
}

bool test_write_png(const std::string &_filename, qivon::Image<unsigned char> &_img) {
  if (_img.isEmpty())
    return false;

  return qivon::writePngFile(_filename, _img);
}

bool test_write_jpg(const std::string &_filename, qivon::Image<unsigned char> &_img,
                    int _quality) {
  if (_img.isEmpty())
    return false;

  return qivon::writeJpgFile(_filename, _img, _quality);
}