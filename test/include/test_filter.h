#ifndef TEST_FILTER_H
#define TEST_FILTER_H

#include <image.h>

bool test_mean_filter_3x3(qivon::Image<unsigned char> &_src, size_t _run_time = 1);

#endif    //TEST_FILTER_H