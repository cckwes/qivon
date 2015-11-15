#ifndef QIVON_JPGIO_H
#define QIVON_JPGIO_H

#include "qivon_config.h"

#include <turbojpeg.h>
#include <image.h>

namespace qivon {
QIVON_EXPORT Image<unsigned char> readJpgFile(const std::string &filename);
}       //namespace qivon

#endif  //QIVON_JPGIO_H