#ifndef QIVON_COLOR_H
#define QIVON_COLOR_H

#include "qivon_config.h"

#include <image.h>

namespace qivon {

QIVON_EXPORT Image<unsigned char> toGrayscale(const Image<unsigned char> &_src);

}

#endif  //QIVON_COLOR_H