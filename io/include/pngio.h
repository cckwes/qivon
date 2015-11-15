#ifndef QIVON_PNGIO_H
#define QIVON_PNGIO_H

#include "qivon_config.h"

#include <png.h>
#include <image.h>

namespace qivon {
QIVON_EXPORT Image<unsigned char> readPngFile(const std::string &filename);

QIVON_EXPORT bool writePngFile(const std::string &filename, Image<unsigned char> &image);
}       //namespace qivon

#endif //QIVON_PNGIO_H
