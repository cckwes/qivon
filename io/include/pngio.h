#ifndef QIVON_PNGIO_H
#define QIVON_PNGIO_H

#include "qivon_config.h"

#include <png.h>
#include <image.h>

namespace qivon {
    Image<unsigned char> QIVON_EXPORT readPngFile(const std::string &filename);
}       //namespace qivon

#endif //QIVON_PNGIO_H
