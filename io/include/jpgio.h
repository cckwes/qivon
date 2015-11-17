#ifndef QIVON_JPGIO_H
#define QIVON_JPGIO_H

#include "qivon_config.h"

#include <turbojpeg.h>
#include <image.h>

namespace qivon {
QIVON_EXPORT Image<unsigned char> readJpgFile(const std::string &filename);

QIVON_EXPORT bool writeJpgFile(const std::string &filename, Image<unsigned char> &image);
}       //namespace qivon

#endif  //QIVON_JPGIO_H