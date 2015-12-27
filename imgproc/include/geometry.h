#ifndef QIVON_GEOMETRY_H
#define QIVON_GEOMETRY_H

#include "qivon_config.h"

#include <image.h>

namespace qivon {

template <class T>
QIVON_EXPORT void resizeImage(Image<T> &_src, Image<T> &_dst,
                              size_t _width, size_t _height,
                              bool _keep_aspect_ratio = true);

//8 bit image
extern template
void resizeImage<unsigned char>(Image<unsigned char> &_src, Image<unsigned char> &_dst,
                                size_t _width, size_t _height, bool _keep_aspect_ratio);

//16 bit image
extern template
void resizeImage<unsigned short>(Image<unsigned short> &_src, Image<unsigned short> &_dst,
                                 size_t width, size_t _height, bool _keep_aspect_ratio);

//32 bit float image
extern template
void resizeImage<float>(Image<float> &_src, Image<float> &_dst,
                        size_t _width, size_t _height, bool _keep_aspect_ratio);

}   //namespace qivon

#endif //QIVON_GEOMETRY_H