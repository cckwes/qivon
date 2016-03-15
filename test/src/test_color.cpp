#include <iostream>
#include <chrono>

#include <color.h>
#include <pngio.h>

#include "timing.h"
#include "test_color.h"

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
    qivon::rgbSwap(_src, bgr);

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
    qivon::rgbSwap(_src, rgb);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() / _run_time << " ms\n";

  if (rgb.isEmpty())
    return false;
  else
    return qivon::writePngFile("bgr_to_rgb.png", rgb);
}

bool test_gamma_correction(qivon::Image<unsigned char> &_src, float _gamma, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> gamma;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::gammaCorrectionLUT(_src, gamma, _gamma);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() / _run_time << " ms\n";

  if (gamma.isEmpty())
    return false;
  else
    return qivon::writePngFile("gamma.png", gamma);
}

bool test_brightness_adjustment(qivon::Image<unsigned char> &_src, int _adjustment, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> brightness;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::brightnessAdjustment(_src, brightness, _adjustment);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() / _run_time << " ms\n";

  if (brightness.isEmpty())
    return false;
  else
    return qivon::writePngFile("brightness.png", brightness);
}

bool test_contrast_adjustment(qivon::Image<unsigned char> &_src, int _adjustment, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> contrast;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::contrastAdjustment(_src, contrast, _adjustment);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() / _run_time << " ms\n";

  if (contrast.isEmpty())
    return false;
  else
    return qivon::writePngFile("contrast.png", contrast);
}

bool test_white_balance_adjustment(qivon::Image<unsigned char> &_src, int _adjustment, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> wb;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::whiteBalanceAdjustment(_src, wb, _adjustment);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() / _run_time << " ms\n";

  if (wb.isEmpty())
    return false;
  else
    return qivon::writePngFile("wb.png", wb);
}

bool test_hue_adjustment(qivon::Image<unsigned char> &_src, int _value, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> hue;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::hueAdjustment(_src, hue, _value);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() / _run_time << " ms\n";

  if (hue.isEmpty())
    return false;
  else
    return qivon::writePngFile("hue.png", hue);
}

bool test_saturation_adjustment(qivon::Image<unsigned char> &_src, int _value, size_t _run_time) {
  if (_src.isEmpty())
    return false;

  qivon::Image<unsigned char> saturation;

  set_start_now();

  for (size_t i = 0; i < _run_time; ++i)
    qivon::saturationAdjustment(_src, saturation, _value);

  std::cout << __FUNCTION__ << " time " << get_elapsed_time() / _run_time << " ms\n";

  if (saturation.isEmpty())
    return false;
  else
    return qivon::writePngFile("saturation.png", saturation);
}