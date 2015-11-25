#if defined(__GNUC__)
#include <x86intrin.h>
#include <iostream>
#elif defined(_MSC_VER)
#include <intrin.h>
#else
#error Unknown compiler, not including any intrinsic header
#endif

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

}