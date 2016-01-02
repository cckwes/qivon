#ifndef QIVON_GEOMETRY_H
#define QIVON_GEOMETRY_H

#include "qivon_config.h"

#include <image.h>

namespace qivon {

//////////////////////////////
// resize image
//////////////////////////////
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
//////////////////////////////


//////////////////////////////
// crop image
//////////////////////////////
template <class T>
QIVON_EXPORT void cropImage(Image<T> &_src, Image<T> &_dst,
                            size_t _start_x, size_t _start_y,
                            size_t _end_x, size_t _end_y);

//8 bit image
extern template
void cropImage(Image<unsigned char> &_src, Image<unsigned char> &_dst,
               size_t _start_x, size_t _start_y,
               size_t _end_x, size_t _end_y);

//16 bit image
extern template
void cropImage(Image<unsigned short> &_src, Image<unsigned short> &_dst,
               size_t _start_x, size_t _start_y,
               size_t _end_x, size_t _end_y);

//32 bit float image
extern template
void cropImage(Image<float> &_src, Image<float> &_dst,
               size_t _start_x, size_t _start_y,
               size_t _end_x, size_t _end_y);
//////////////////////////////

}   //namespace qivon

#endif //QIVON_GEOMETRY_H