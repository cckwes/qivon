#if defined(__GNUC__)
#include <x86intrin.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#else
#error Unknown compiler, not including any intrinsic header
#endif

#include "color.h"

namespace qivon {

void rgb_to_grayscale_8u(unsigned char* _src, unsigned char* _dst, int _width, int _height) {
  unsigned char *r_muliply = (unsigned char *) malloc(_width * _height);
  unsigned char *g_muliply = (unsigned char *) malloc(_width * _height);
  unsigned char *b_muliply = (unsigned char *) malloc(_width * _height);

  const int img_size = _width * _height;

  for (int i = 0; i < img_size; ++i) {
    r_muliply[i] = (unsigned char) ((float) _src[i] * 0.2126f);
    g_muliply[i] = (unsigned char) ((float) _src[i + img_size] * 0.7152f);
    b_muliply[i] = (unsigned char) ((float) _src[i + 2 * img_size] * 0.0722f);
  }

  int length = img_size;

  int start_of_remaining = 0;
  for (int i = 0; i < (length - 16); i += 16) {
    __m128i r_vec = _mm_loadu_si128((__m128i*) &r_muliply[i]);
    __m128i g_vec = _mm_loadu_si128((__m128i*) &g_muliply[i]);
    __m128i b_vec = _mm_loadu_si128((__m128i*) &b_muliply[i]);

    __m128i tmp_vec = _mm_add_epi8(r_vec, g_vec);
    __m128i result_vec = _mm_add_epi8(tmp_vec, tmp_vec);

    unsigned char *result = (unsigned char *) &result_vec;

    for (int j = 0; j < 16; ++j) {
      _dst[i + j] = result[j];
    }

    start_of_remaining = i + 16;
  }

  int remaining = length % 16;

  if (remaining != 0) {
    for (int i = start_of_remaining; i < length; ++i) {
      _dst[i] = r_muliply[i] + g_muliply[i] + b_muliply[i];
    }
  }
}

void toGrayscale(Image<unsigned char> &_src, Image<unsigned char> &_dst)
{
}

}