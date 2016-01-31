#ifndef QIVON_COLOR_H
#define QIVON_COLOR_H

#include "qivon_config.h"

#include <image.h>

namespace qivon {

QIVON_EXPORT void toGrayscale(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void rgb_to_bgr(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void bgr_to_rgb(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void rgb_to_hsv(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void rgb_to_hsl(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void hsl_to_rgb(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void hsv_to_rgb(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void rgb_to_yuv(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void yuv_to_rgb(Image<unsigned char> &_src, Image<unsigned char> &_dst);

QIVON_EXPORT void gamma_correction(Image<unsigned char> &_src,
                                   Image<unsigned char> &_dst,
                                   float _gamma);

QIVON_EXPORT void gamma_correction_LUT(Image<unsigned char> &_src,
                                       Image<unsigned char> &_dst,
                                       float _gamma);

QIVON_EXPORT void brightness_adjustment(Image<unsigned char> &_src,
                                        Image<unsigned char> &_dst,
                                        int _adjustment);

QIVON_EXPORT void contrast_adjustment(Image<unsigned char> &_src,
                                      Image<unsigned char> &_dst,
                                      int _adjustment);

QIVON_EXPORT bool white_balance_adjustment(Image<unsigned char> &_src,
                                           Image<unsigned char> &_dst,
                                           int temperature);

QIVON_EXPORT bool hue_adjustment(Image<unsigned char> &_src, Image<unsigned char> &_dst, int _value);

QIVON_EXPORT bool saturation_adjustment(Image<unsigned char> &_src, Image<unsigned char> &_dst, int _value);
}

#endif  //QIVON_COLOR_H