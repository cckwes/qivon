#include <iostream>
#include <chrono>

#include <color.h>
#include <pngio.h>

#include "test_color.h"

std::chrono::system_clock::time_point g_start;

void set_start_now() {
  g_start = std::chrono::system_clock::now();
}

long long get_elapsed_time() {
  auto elapsed =  std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::system_clock::now() - g_start);
  return elapsed.count();
}

bool test_to_grayscale(qivon::Image<unsigned char> &_src, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> grayscale;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::toGrayscale(_src, grayscale);

  std::cout << __FUNCTION__ << " time: " << get_elapsed_time() / _run_time << " ms\n";

  if (grayscale.isEmpty())
    return false;
  else
    return qivon::writePngFile("grayscale.png", grayscale);
}

bool test_rgb_to_bgr(qivon::Image<unsigned char> &_src, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> bgr;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::rgb_to_bgr(_src, bgr);

  std::cout << __FUNCTION__ << " time: " << get_elapsed_time() / _run_time << " ms\n";

  if (bgr.isEmpty())
    return false;
  else
    return qivon::writePngFile("bgr.png", bgr);
}

bool test_bgr_to_rgb(qivon::Image<unsigned char> &_src, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> rgb;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::bgr_to_rgb(_src, rgb);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() / _run_time << " ms\n";

  if (rgb.isEmpty())
    return false;
  else
    return qivon::writePngFile("bgr_to_rgb.png", rgb);
}