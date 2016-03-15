#ifndef QIVON_COLOR_H
#define QIVON_COLOR_H

#include "qivon_config.h"

#include <image.h>

namespace qivon {

using u8_image = Image<unsigned char>;

QIVON_EXPORT void toGrayscale(u8_image& src, u8_image& dst);

QIVON_EXPORT void rgbSwap(u8_image& src, u8_image& dst);

QIVON_EXPORT void rgb2Hsv(u8_image& src, u8_image& dst);

QIVON_EXPORT void rgb2Hsl(u8_image& src, u8_image& dst);

QIVON_EXPORT void hsl2Rgb(u8_image& src, u8_image& dst);

QIVON_EXPORT void hsv2Rgb(u8_image& src, u8_image& dst);

QIVON_EXPORT void rgb2Yuv(u8_image& src, u8_image& dst);

QIVON_EXPORT void yuv2Rgb(u8_image& src, u8_image& dst);

QIVON_EXPORT void gammaCorrection(u8_image& src,
                                  u8_image& dst,
                                  float _gamma);

QIVON_EXPORT void gammaCorrectionLUT(u8_image& src,
                                       u8_image& dst,
                                       float _gamma);

QIVON_EXPORT void brightnessAdjustment(u8_image& src,
                                        u8_image& dst,
                                        int _adjustment);

QIVON_EXPORT void contrastAdjustment(u8_image& src,
                                      u8_image& dst,
                                      int _adjustment);

QIVON_EXPORT bool whiteBalanceAdjustment(u8_image& src,
                                           u8_image& dst,
                                           int temperature);

QIVON_EXPORT bool hueAdjustment(u8_image& src, u8_image& dst, int _value);

QIVON_EXPORT bool saturationAdjustment(u8_image& src, u8_image& dst, int _value);
}

#endif  //QIVON_COLOR_H