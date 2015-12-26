#ifndef QIVON_GEOMETRY_H
#define QIVON_GEOMETRY_H

#include "qivon_config.h"

#include <image.h>

namespace qivon {

template <class T>
QIVON_EXPORT void resizeImage(Image<T> &_src, Image<T> &_dst,
                              size_t _width, size_t _height,
                              bool _keep_aspect_ratio = true);

extern template
void resizeImage<unsigned char>(Image<unsigned char> &_src, Image<unsigned char> &_dst,
                                size_t _width, size_t _height, bool _keep_aspect_ratio);

}   //namespace qivon

#endif //QIVON_GEOMETRY_H