#ifndef TEST_COLOR_H
#define TEST_COLOR_H

#include <string>
#include <image.h>

bool test_to_grayscale(qivon::Image<unsigned char> &_src, size_t _run_time = 1);

bool test_rgb_to_bgr(qivon::Image<unsigned char> &_src, size_t _run_time = 1);

bool test_bgr_to_rgb(qivon::Image<unsigned char> &_src, size_t _run_time = 1);

bool test_gamma_correction(qivon::Image<unsigned char> &_src, float _gamma, size_t _run_time = 1);

#endif    //TEST_COLOR_H