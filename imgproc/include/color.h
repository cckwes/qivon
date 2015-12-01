#ifndef QIVON_COLOR_H
#define QIVON_COLOR_H

#include "qivon_config.h"

#include <image.h>

namespace qivon {

QIVON_EXPORT void toGrayscale(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void rgb_to_bgr(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void bgr_to_rgb(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void gamma_correction(Image<unsigned char> &_src, Image<unsigned char> &_dst, float _gamma);

}

#endif  //QIVON_COLOR_H