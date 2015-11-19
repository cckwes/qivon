#ifndef QIVON_COLOR_H
#define QIVON_COLOR_H

#include "qivon_config.h"

#include <image.h>

namespace qivon {

template <class T>
QIVON_EXPORT void toGrayscale(Image<T> &_src, Image<T> &_dst);

}

#endif  //QIVON_COLOR_H