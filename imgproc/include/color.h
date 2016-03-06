#ifndef QIVON_COLOR_H
#define QIVON_COLOR_H

#include "qivon_config.h"

#include <image.h>

namespace qivon {

using u8_image = Image<unsigned char>;

QIVON_EXPORT void toGrayscale(u8_image &_src, u8_image &_dst);

QIVON_EXPORT void rgb_to_bgr(u8_image &_src, u8_image &_dst);

QIVON_EXPORT void bgr_to_rgb(u8_image &_src, u8_image &_dst);

QIVON_EXPORT void rgb_to_hsv(u8_image &_src, u8_image &_dst);

QIVON_EXPORT void rgb_to_hsl(u8_image &_src, u8_image &_dst);

QIVON_EXPORT void hsl_to_rgb(u8_image &_src, u8_image &_dst);

QIVON_EXPORT void hsv_to_rgb(u8_image &_src, u8_image &_dst);

QIVON_EXPORT void rgb_to_yuv(u8_image &_src, u8_image &_dst);

QIVON_EXPORT void yuv_to_rgb(u8_image &_src, u8_image &_dst);

QIVON_EXPORT void gamma_correction(u8_image &_src,
                                   u8_image &_dst,
                                   float _gamma);

QIVON_EXPORT void gamma_correction_LUT(u8_image &_src,
                                       u8_image &_dst,
                                       float _gamma);

QIVON_EXPORT void brightness_adjustment(u8_image &_src,
                                        u8_image &_dst,
                                        int _adjustment);

QIVON_EXPORT void contrast_adjustment(u8_image &_src,
                                      u8_image &_dst,
                                      int _adjustment);

QIVON_EXPORT bool white_balance_adjustment(u8_image &_src,
                                           u8_image &_dst,
                                           int temperature);

QIVON_EXPORT bool hue_adjustment(u8_image &_src, u8_image &_dst, int _value);

QIVON_EXPORT bool saturation_adjustment(u8_image &_src, u8_image &_dst, int _value);
}

#endif  //QIVON_COLOR_H