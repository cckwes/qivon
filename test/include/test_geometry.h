#ifndef TEST_GEOMETRY_H
#define TEST_GEOMETRY_H

#include <image.h>

bool test_resize_image(qivon::Image<unsigned char> &_src, size_t _width, size_t _height);

bool test_crop_image(qivon::Image<unsigned char> &_src,
                     size_t _start_x, size_t _start_y, size_t _end_x, size_t _end_y);

#endif    //TEST_GEOMETRY_H