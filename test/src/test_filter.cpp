#include <iostream>
#include <chrono>

#include <filter.h>
#include <pngio.h>

#include "timing.h"
#include "test_filter.h"

bool test_mean_filter_3x3(qivon::Image<unsigned char> &_src, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> mean;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::meanFilter3x3(_src, mean);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() / _run_time << " ms\n";

  if (mean.isEmpty())
    return false;
  else
    return qivon::writePngFile("mean.png", mean);
}