#if defined(__GNUC__)
#include <x86intrin.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#else
#error Unknown compiler, not including any intrinsic header
#endif

#include <iostream>
#include <cmath>
#include "color.h"

namespace qivon {

void rgb_to_grayscale_8u(unsigned char* _src, unsigned char* _dst, size_t _width, size_t _height) {
  unsigned char *r_muliply = (unsigned char *) malloc(_width * _height);
  unsigned char *g_muliply = (unsigned char *) malloc(_width * _height);
  unsigned char *b_muliply = (unsigned char *) malloc(_width * _height);

  const size_t img_size = _width * _height;

  for (int i = 0; i < img_size; ++i) {
    r_muliply[i] = (unsigned char) ((float) _src[i] * 0.2126f);
    g_muliply[i] = (unsigned char) ((float) _src[i + img_size] * 0.7152f);
    b_muliply[i] = (unsigned char) ((float) _src[i + 2 * img_size] * 0.0722f);
  }

  size_t length = img_size;

  int start_of_remaining = 0;
  for (int i = 0; i < (length - 16); i += 16) {
    __m128i r_vec = _mm_loadu_si128((__m128i*) &r_muliply[i]);
    __m128i g_vec = _mm_loadu_si128((__m128i*) &g_muliply[i]);
    __m128i b_vec = _mm_loadu_si128((__m128i*) &b_muliply[i]);

    __m128i tmp_vec = _mm_add_epi8(r_vec, g_vec);
    __m128i result_vec = _mm_add_epi8(tmp_vec, b_vec);

    unsigned char *result = (unsigned char *) &result_vec;

    for (int j = 0; j < 16; ++j) {
      _dst[i + j] = result[j];
    }

    start_of_remaining = i + 16;
  }

  size_t remaining = length % 16;

  if (remaining != 0) {
    for (int i = start_of_remaining; i < length; ++i) {
      _dst[i] = r_muliply[i] + g_muliply[i] + b_muliply[i];
    }
  }
}

void bgr_to_grayscale_8u(unsigned char* _src, unsigned char* _dst, size_t _width, size_t _height) {
  unsigned char *r_muliply = (unsigned char *) malloc(_width * _height);
  unsigned char *g_muliply = (unsigned char *) malloc(_width * _height);
  unsigned char *b_muliply = (unsigned char *) malloc(_width * _height);

  const size_t img_size = _width * _height;

  for (int i = 0; i < img_size; ++i) {
    b_muliply[i] = (unsigned char) ((float) _src[i] * 0.0722f);
    g_muliply[i] = (unsigned char) ((float) _src[i + img_size] * 0.7152f);
    r_muliply[i] = (unsigned char) ((float) _src[i + 2 * img_size] * 0.2126f);
  }

  size_t length = img_size;

  int start_of_remaining = 0;
  for (int i = 0; i < (length - 16); i += 16) {
    __m128i r_vec = _mm_loadu_si128((__m128i*) &r_muliply[i]);
    __m128i g_vec = _mm_loadu_si128((__m128i*) &g_muliply[i]);
    __m128i b_vec = _mm_loadu_si128((__m128i*) &b_muliply[i]);

    __m128i tmp_vec = _mm_add_epi8(r_vec, g_vec);
    __m128i result_vec = _mm_add_epi8(tmp_vec, b_vec);

    unsigned char *result = (unsigned char *) &result_vec;

    for (int j = 0; j < 16; ++j) {
      _dst[i + j] = result[j];
    }

    start_of_remaining = i + 16;
  }

  size_t remaining = length % 16;

  if (remaining != 0) {
    for (int i = start_of_remaining; i < length; ++i) {
      _dst[i] = r_muliply[i] + g_muliply[i] + b_muliply[i];
    }
  }
}

void toGrayscale(Image<unsigned char> &_src, Image<unsigned char> &_dst) {
  //can only handle RGB type
  if (_src.color() != Type_RGB
      && _src.color() != Type_BGR
      && _src.color() != Type_RGBA) {
    std::cerr << "Can not convert image type other than RGBA, RGB and BGRin "
        << __FUNCTION__ << " at " << __LINE__ << "\n";
    return;
  }

  if (_src.data() == nullptr
      || _src.width() == 0
      || _src.height() == 0
      || _src.channels() == 0) {
    std::cerr << "source image nullptr or wrong size in "
        << __FUNCTION__ << " at " << __LINE__ << "\n";
    return;
  }

  unsigned char *gray = (unsigned char*) malloc(_src.width() * _src.height() * sizeof(unsigned char));

  if (_src.color() == Type_RGB || _src.color() == Type_RGBA)
    rgb_to_grayscale_8u(_src.data(), gray, _src.width(), _src.height());
  else
    bgr_to_grayscale_8u(_src.data(), gray, _src.width(), _src.height());

  _dst = Image<unsigned char>(_src.width(), _src.height(), 1, Type_Grayscale, gray);
}

void rgb_to_bgr(Image<unsigned char> &_src, Image<unsigned char> &_dst) {
  //input can be only type rgb
  if (_src.color() != Type_RGB) {
    std::cerr << "Can not convert image type other than RGB in "
        << __FUNCTION__ << " at line " << __LINE__ << "\n";
    return;
  }

  if (_src.data() == nullptr
      || _src.width() == 0
      || _src.height() == 0
      || _src.channels() == 0) {
    std::cerr << "source image nullptr or wrong size in "
        << __FUNCTION__ << " at line " << __LINE__ << "\n";
    return;
  }

  size_t image_size = _src.width() * _src.height();

  unsigned char *bgr = (unsigned char*) malloc(3 * _src.width() * _src.height() * sizeof(unsigned char));
  unsigned char *original = _src.data();

  for (size_t i = 0; i < image_size; ++i) {
    bgr[i] = original[2 * image_size + i];
    bgr[image_size + i] = original[image_size + i];
    bgr[2 * image_size + i] = original[i];
  }

  _dst = Image<unsigned char>(_src.width(), _src.height(), 3, Type_BGR, bgr);
}

void bgr_to_rgb(Image<unsigned char> &_src, Image<unsigned char> &_dst) {
  //input can be only type bgr
  if (_src.color() != Type_BGR) {
    std::cerr << "Can not convert image type other than BGR in "
        << __FUNCTION__ << " at line " << __LINE__ << "\n";
    return;
  }

  if (_src.data() == nullptr
      || _src.width() == 0
      || _src.height() == 0
      || _src.channels() == 0) {
    std::cerr << "source image nullptr or wrong size in "
        << __FUNCTION__ << " at line " << __LINE__ << "\n";
    return;
  }

  size_t image_size = _src.width() * _src.height();

  unsigned char *rgb = (unsigned char*) malloc(3 * _src.width() * _src.height() * sizeof(unsigned char));
  unsigned char *original = _src.data();

  for (size_t i = 0; i < image_size; ++i) {
    rgb[i] = original[2 * image_size + i];
    rgb[image_size + i] = original[image_size + i];
    rgb[2 * image_size + i] = original[i];
  }

  _dst = Image<unsigned char>(_src.width(), _src.height(), 3, Type_RGB, rgb);
}

void rgb_to_hsv(Image<unsigned char> &_src, Image<unsigned char> &_dst) {
  //check image empty
  if (_src.isEmpty()) {
    std::cerr << "empty image in " << __FUNCTION__ << std::endl;
    return;
  }

  //check input image color type
  if (_src.color() != Type_RGB || _src.channels() != 3) {
    std::cerr << "input color type not rgb in " << __FUNCTION__ << std::endl;
    return;
  }

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  unsigned char *source_data = _src.data();

  unsigned char *result = (unsigned char *) malloc(img_size * 3 * sizeof(unsigned char));

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      int r, g, b;
      r = (float) source_data[i * width + j] / 255.0f;
      g = (float) source_data[i * width + j + img_size] / 255.0f;
      b = (float) source_data[i * width + j + img_size + img_size] / 255.0f;

      float min, max, delta;
      min = (float) std::min(r, std::min(g, b));
      max = (float) std::max(r, std::max(g, b));
      delta = max - min;

      //get the V value
      unsigned char v = (unsigned char) (max * 255.0f);

      //get the H value
      float f_h;
      if (delta == 0)
        f_h = 0;
      else if (max == r)
        f_h = 60.0f * (g - b) / delta;
      else if (max == g)
        f_h = 120.0f + 60.0f * (b - r) / delta;
      else                              //max == b
        f_h = 240.0f + 60.0f * (r - g) / delta;
      if (f_h < 0)
        f_h += 360.0f;

      unsigned char h = (unsigned char) f_h / 360.0f * 255.0f;

      //get the S value
      unsigned char s;
      if (max == 0)
        s = 0;
      else
        s = (unsigned char) (delta / max) * 255.0f;

      //put HSV value into result buffer
      result[i * width + j] = h;
      result[i * width + j + img_size] = s;
      result[i * width + j + img_size + img_size] = v;
    }
  }

  _dst = qivon::Image<unsigned char>(width, height, 3, qivon::Type_HSV, result);
}

void build_gamma_correction_LUT(unsigned char *table, float _gamma_correction) {
  for (size_t i = 0; i < 256; ++i) {
    table[i] = (unsigned char) (255.0f * std::pow((float)i / 255.0f, _gamma_correction));
  }
}

void gamma_correction(Image<unsigned char> &_src, Image<unsigned char> &_dst, float _gamma) {
  if (_src.isEmpty()) {
    std::cerr << "source image empty\n";
    return;
  }

  float gamma_correction = 1.0f / _gamma;

  unsigned char *result = (unsigned char *) malloc(_src.channels() * _src.width()
                                                       * _src.height() * sizeof(unsigned char));

  size_t image_size = _src.width() * _src.height() * _src.channels();

  for (size_t i = 0; i < image_size; ++i) {
    result[i] = (unsigned char) (255.0f * std::pow(_src.data()[i] / 255.0f, gamma_correction));
  }

  _dst = Image<unsigned char>(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

void gamma_correction_LUT(Image<unsigned char> &_src, Image<unsigned char> &_dst, float _gamma) {
  if (_src.isEmpty()) {
    std::cerr << "source image empty\n";
    return;
  }

  float gamma_correction = 1.0f / _gamma;

  size_t image_size = _src.width() * _src.height() * _src.channels();

  unsigned char table[256];

  build_gamma_correction_LUT(table, gamma_correction);

  unsigned char *result = (unsigned char *) malloc(image_size);

  for (size_t i = 0; i < image_size; ++i) {
    result[i] = table[_src.data()[i]];
  }

  _dst = Image<unsigned char>(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

void increase_brightness(Image<unsigned char> &_src,
                         Image<unsigned char> &_dst,
                         unsigned char _value) {
  const size_t image_size = _src.width() * _src.height() * _src.channels();

  unsigned char *result = (unsigned char *) malloc(image_size);

  __m128i value = _mm_set1_epi8(_value);

  size_t i;
  for (i = 0; i < (image_size - 16); i += 16) {
    __m128i src_vec = _mm_loadu_si128((__m128i*) &_src.data()[i]);
    __m128i result_vec = _mm_adds_epu8(src_vec, value);

    unsigned char *res = (unsigned char *) &result_vec;

    for (size_t j = 0; j < 16; ++j) {
      result[i + j] = res[j];
    }
  }

  for (; i < image_size; ++i) {
    result[i] = _src.data()[i] + _value;
  }

  _dst = Image<unsigned char>(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

void decrease_brightness(Image<unsigned char> &_src,
                         Image<unsigned char> &_dst,
                         unsigned char _value) {
  const size_t image_size = _src.width() * _src.height() * _src.channels();

  unsigned char *result = (unsigned char *) malloc(image_size);

  __m128i value = _mm_set1_epi8(_value);

  size_t i;
  for (i = 0; i < (image_size - 16); i += 16) {
    __m128i src_vec = _mm_loadu_si128((__m128i*) &_src.data()[i]);
    __m128i result_vec = _mm_subs_epu8(src_vec, value);

    unsigned char *res = (unsigned char *) &result_vec;

    for (size_t j = 0; j < 16; ++j) {
      result[i + j] = res[j];
    }
  }

  for (; i < image_size; ++i) {
    result[i] = _src.data()[i] - _value;
  }

  _dst = Image<unsigned char>(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

void  brightness_adjustment(Image<unsigned char> &_src,
                           Image<unsigned char> &_dst,
                           int _adjustment) {
  if (_src.isEmpty()) {
    std::cerr << "source image empty\n";
    _dst = Image<unsigned char>();
    return;
  }

  if (_adjustment > 255 || _adjustment < -255) {
    std::cerr << "invalid brightness adjustment value\n";
    _dst = Image<unsigned char>();
    return;
  }

  if (_adjustment > 0) {
    increase_brightness(_src, _dst, (unsigned char) _adjustment);
  } else {
    decrease_brightness(_src, _dst, (unsigned char) _adjustment);
  }
}

void contrast_adjustment(Image<unsigned char> &_src,
                         Image<unsigned char> &_dst,
                         int _adjustment) {
  if (_src.isEmpty()) {
    std::cerr << "source image empty\n";
    _dst = Image<unsigned char>();
    return;
  }

  if (_adjustment > 255 || _adjustment < -255) {
    std::cerr << "invalid contrast adjustment value\n";
    _dst = Image<unsigned char>();
    return;
  }

  unsigned char lut[256];

  float factor = 259.0f * float(_adjustment + 255) / (255.0f * float(259 - _adjustment));

  //build the LUT
  for (size_t i = 0; i < 256; ++i) {
    lut[i] = (unsigned char) std::max(std::min((int) (factor * (i - 128) + 128), 255), 0);
  }

  size_t image_size = _src.width() * _src.height() * _src.channels();

  unsigned char *result = (unsigned char *) malloc(image_size);
  unsigned char *source = _src.data();

  for (size_t j = 0; j < image_size; ++j) {
    result[j] = lut[source[j]];
  }

  _dst = Image<unsigned char>(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

bool white_balance_adjustment(Image<unsigned char> &_src,
                              Image<unsigned char> &_dst,
                              int temperature) {
  //check temperature out of range
  if (temperature < 1000 || temperature > 20000) {
    std::cerr << "temperature out of range in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check image empty
  if (_src.isEmpty()) {
    std::cerr << "source image empty in " << __FUNCTION__ << std::endl;
    return false;
  }

  //image must be RGB type
  if (_src.color() != qivon::Type_RGB) {
    std::cerr << "image must be RGB type in " << __FUNCTION__ << std::endl;
    return false;
  }

  float temp = (float) temperature / 100.0f;

  //calculate the 3 channel ratio
  unsigned char red_val, green_val, blue_val;
  red_val = green_val = blue_val = 0;

  float a, b, c;

  //red value calculation
  if (temp <= 66)
    red_val = 255;
  else {
    a = 351.9769;
    b = 0.1142;
    c = -40.2537;
    float x = temp - 55;
    float result = a + b * x + c * std::log(x);
    red_val = std::max(0, std::min((int) result, 255));
  }

  //green value calculation
  if (temp < 66) {
    a = -155.2549;
    b = -0.4460;
    c = 104.4922;
    float x = temp - 2;
    float result = a + b * x + c * std::log(x);
    green_val = std::max(0, std::min((int) result, 255));
  } else {
    a = 325.4494;
    b = 0.0794;
    c = -28.0853;
    float x = temp - 50;
    float result = a + b * x + c * std::log(x);
    green_val = std::max(0, std::min((int) result, 255));
  }

  //blue value calculation
  if (temp >= 66)
    blue_val = 255;
  else if (temp <= 20)
    blue_val = 0;
  else {
    a = -254.7694;
    b = 0.8274;
    c = 115.6799;
    float x = temp - 10;
    float result = a + b * x + c * std::log(x);
    blue_val = std::max(0, std::min((int) result, 255));
  }

  float red_ratio = (float) red_val / 255.0f;
  float green_ratio = (float) green_val / 255.0f;
  float blue_ratio = (float) blue_val / 255.0f;

  //built the LUT
  unsigned char r_lut[256], g_lut[256], b_lut[256];
  for (int i = 0; i < 256; ++i) {
    r_lut[i] = (unsigned char) ((float) i * red_ratio);
    g_lut[i] = (unsigned char) ((float) i * green_ratio);
    b_lut[i] = (unsigned char) ((float) i * blue_ratio);
  }

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  unsigned char *source_data = _src.data();

  unsigned char *result = (unsigned char*) malloc(img_size * _src.channels());

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      result[i * width + j] = r_lut[source_data[i * width + j]];
      result[i * width + j + img_size] = g_lut[source_data[i * width + j + img_size]];
      result[i * width + j + img_size + img_size] = b_lut[source_data[i * width + j + img_size + img_size]];
    }
  }

  _dst = qivon::Image<unsigned char>(width, height, _src.channels(), _src.color(), result);

  return true;
}

}