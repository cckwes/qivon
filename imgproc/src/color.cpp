#if defined(__GNUC__)
#include <x86intrin.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#else
#error Unknown compiler, not including any intrinsic header
#endif

#include <iostream>
#include <cmath>
#include <algorithm>
#include "color.h"

namespace qivon {

void rgb2Grayscale(unsigned char* src,
                   unsigned char* dst,
                   size_t width,
                   size_t height,
                   bool rgb = true) {
  unsigned char* const r_muliply = (unsigned char *) malloc(width * height);
  unsigned char* const g_muliply = (unsigned char *) malloc(width * height);
  unsigned char* const b_muliply = (unsigned char *) malloc(width * height);

  const size_t kImgSize = width * height;

  const float r_factor = 0.2126f;
  const float g_factor = 0.7152f;
  const float b_factor = 0.0722f;

  for (size_t i = 0; i < kImgSize; ++i) {
    r_muliply[i] = (unsigned char) (r_factor * (rgb ? src[i] : src[i + kImgSize + kImgSize]));
    g_muliply[i] = (unsigned char) (g_factor * src[i + kImgSize]);
    b_muliply[i] = (unsigned char) (b_factor * (rgb ? src[i + kImgSize + kImgSize] : src[i]));
  }

  int start_of_remaining = 0;
  const size_t vector_length = 16;

  for (size_t i = 0; i <= (kImgSize - vector_length); i += vector_length) {
    __m128i r_vec = _mm_loadu_si128((__m128i*) &r_muliply[i]);
    __m128i g_vec = _mm_loadu_si128((__m128i*) &g_muliply[i]);
    __m128i b_vec = _mm_loadu_si128((__m128i*) &b_muliply[i]);

    __m128i tmp_vec = _mm_add_epi8(r_vec, g_vec);
    __m128i result_vec = _mm_add_epi8(tmp_vec, b_vec);

    unsigned char* const result = (unsigned char *) &result_vec;

    std::copy(result, result + vector_length, &dst[i]);

    start_of_remaining = i + vector_length;
  }

  const size_t remaining = kImgSize % vector_length;

  if (remaining != 0) {
    for (size_t i = start_of_remaining; i < kImgSize; ++i) {
      dst[i] = r_muliply[i] + g_muliply[i] + b_muliply[i];
    }
  }

  free(r_muliply);
  free(g_muliply);
  free(b_muliply);
}

void toGrayscale(u8_image &src, u8_image &dst) {
  //can only handle RGB type
  if (src.color() != Type_RGB
      && src.color() != Type_BGR
      && src.color() != Type_RGBA) {
    std::cerr << "Can not convert image type other than RGBA, RGB and BGR in "
        << __FUNCTION__ << " at " << __LINE__ << "\n";
    return;
  }

  if (src.isEmpty()) {
    std::cerr << "source image nullptr or wrong size in "
        << __FUNCTION__ << " at " << __LINE__ << "\n";
    return;
  }

  unsigned char* const gray = (unsigned char*) malloc(src.width() * src.height() * sizeof(unsigned char));

  if (src.color() == Type_RGB || src.color() == Type_RGBA)
    rgb2Grayscale(src.data(), gray, src.width(), src.height());
  else
    rgb2Grayscale(src.data(), gray, src.width(), src.height(), false);

  dst = u8_image(src.width(), src.height(), 1, Type_Grayscale, gray);
}

void rgbSwap(u8_image &src, u8_image &dst) {
  if (src.color() != Type_RGB && src.color() != Type_BGR) {
    std::cerr << "Can only swap image type RGB or BGR in "
        << __FUNCTION__ << " at line " << __LINE__ << "\n";
    return;
  }

  if (src.isEmpty()) {
    std::cerr << "Source image empty in "
        << __FUNCTION__ << " at line " << __LINE__ << "\n";
    return;
  }

  const size_t kImgSize = src.width() * src.height();

  unsigned char* const result = (unsigned char*) malloc(3 * kImgSize * sizeof(unsigned char));
  const unsigned char* const original = src.data();

  //swap the 0th channel
  std::copy(original, original + kImgSize, result + 2 * kImgSize);

  //copy the 1st channel
  std::copy(original + kImgSize, original + 2 * kImgSize, result + kImgSize);

  //swap the 2nd channel
  std::copy(original + 2 * kImgSize, original + 3 * kImgSize, result);

  dst = u8_image(src.width(), src.height(), 3,
                 (src.color() == Type_RGB) ? Type_BGR : Type_RGB, result);
}

void rgb_to_hsv(u8_image &src, u8_image &dst) {
  //check image empty
  if (src.isEmpty()) {
    std::cerr << "Source image empty in " << __FUNCTION__ << std::endl;
    return;
  }

  //check input image color type
  if (src.color() != Type_RGB || src.channels() != 3) {
    std::cerr << "input color type not rgb in " << __FUNCTION__ << std::endl;
    return;
  }

  const size_t& kWidth = src.width();
  const size_t& kHeight = src.height();
  const size_t kImgSize = kWidth * kHeight;
  const unsigned char* const original = src.data();

  unsigned char* const result = (unsigned char*) malloc(kImgSize * 3 * sizeof(unsigned char));

  for (size_t i = 0; i < kHeight; ++i) {
    for (size_t j = 0; j < kWidth; ++j) {
      float r = float(original[i * kWidth + j]) / 255.0f;
      float g = float(original[i * kWidth + j + kImgSize]) / 255.0f;
      float b = float(original[i * kWidth + j + 2 * kImgSize]) / 255.0f;

      float min = std::min(r, std::min(g, b));
      float max = std::max(r, std::max(g, b));
      float delta = max - min;

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
      unsigned char s = (max == 0) ? 0 : (unsigned char) ((delta / max) * 255.0f);

      //put HSV value into result buffer
      result[i * kWidth + j] = h;
      result[i * kWidth + j + kImgSize] = s;
      result[i * kWidth + j + 2 * kImgSize] = v;
    }
  }

  dst = qivon::u8_image(kWidth, kHeight, 3, qivon::Type_HSV, result);
}

void hsv_to_rgb(u8_image &src, u8_image &dst) {
  //check input image empty
  if (src.isEmpty()) {
    std::cerr << "Source image empty in " << __FUNCTION__ << std::endl;
    return;
  }

  //check input color type and number of channels
  if (src.color() != qivon::Type_HSV || src.channels() != 3) {
    std::cerr << "input color type not hsv in " << __FUNCTION__ << std::endl;
    return;
  }

  const size_t& kWidth = src.width();
  const size_t& kHeight = src.height();
  const size_t kImgSize = kWidth * kHeight;
  const unsigned char* const original = src.data();

  unsigned char* const result = (unsigned char*) malloc(kImgSize * 3 * sizeof(unsigned char));

  for (size_t i = 0; i < kHeight; ++i) {
    for (size_t j = 0; j < kWidth; ++j) {
      float h = float(original[i * kWidth + j]) * 1.4117647f;   //is /255*360
      float s = float(original[i * kWidth + j + kImgSize]) / 255.0f;
      float v = float(original[i * kWidth + j + 2 * kImgSize]) / 255.0f;

      unsigned char r, g, b;
      if (s == 0) {
        r = g = b = 0;
      } else {
        int sector = std::floor(h / 60.0f);
        float f = h / 60.0f - sector;
        float p = v * (1.0f - s);
        float q = v * (1.0f - s * f);
        float t = v * (1.0f - s * (1.0f - f));

        switch (sector) {
          case 0:
            r = (unsigned char) (v * 255.0f);
            g = (unsigned char) (t * 255.0f);
            b = (unsigned char) (p * 255.0f);
            break;

          case 1:
            r = (unsigned char) (q * 255.0f);
            g = (unsigned char) (v * 255.0f);
            b = (unsigned char) (p * 255.0f);
            break;

          case 2:
            r = (unsigned char) (p * 255.0f);
            g = (unsigned char) (v * 255.0f);
            b = (unsigned char) (t * 255.0f);
            break;

          case 3:
            r = (unsigned char) (p * 255.0f);
            g = (unsigned char) (q * 255.0f);
            b = (unsigned char) (v * 255.0f);
            break;

          case 4:
            r = (unsigned char) (t * 255.0f);
            g = (unsigned char) (p * 255.0f);
            b = (unsigned char) (v * 255.0f);
            break;

          default:
            r = (unsigned char) (v * 255.0f);
            g = (unsigned char) (p * 255.0f);
            b = (unsigned char) (q * 255.0f);
            break;
        }
      }

      result[i * kWidth + j] = r;
      result[i * kWidth + j + kImgSize] = g;
      result[i * kWidth + j + 2 * kImgSize] = b;
    }
  }

  dst = u8_image(kWidth, kHeight, 3, Type_RGB, result);
}

void rgb_to_hsl(u8_image &_src, u8_image &_dst) {
  //check image empty
  if (_src.isEmpty()) {
    std::cerr << "empty image in " << __FUNCTION__ << std::endl;
    return;
  }

  //check image type and channels
  if (_src.color() != Type_RGB || _src.channels() != 3) {
    std::cerr << "input color type not rgb in " << __FUNCTION__ << std::endl;
    return;
  }

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  unsigned char *source_data = _src.data();
  unsigned char *result = (unsigned char *) malloc(img_size * 3);

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      float r, g, b;
      r = float(source_data[i * width + j]) / 255.0f;
      g = float(source_data[i * width + j + img_size]) / 255.0f;
      b = float(source_data[i * width + j + img_size + img_size]) / 255.0f;

      float min, max, delta;
      min = std::min(r, std::min(g, b));
      max = std::max(r, std::max(g, b));
      delta = max - min;

      //get the L value
      unsigned char l = (unsigned char) ((max + min) / 2.0f * 255.0f);

      //get the S value
      unsigned char s;
      if (delta == 0)
        s = 0;
      else
        s = (unsigned char) (delta / 1.0 - std::fabsf(max + min - 1));

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

      //put HSL into result buffer
      size_t data_loc = i * width + j;
      result[data_loc] = h;
      data_loc += img_size;
      result[data_loc] = s;
      data_loc += img_size;
      result[data_loc] = l;
    }
  }

  _dst = u8_image(width, height, 3, Type_HSL, result);
}

void hsl_to_rgb(u8_image &_src, u8_image &_dst) {
  //check image empty
  if (_src.isEmpty()) {
    std::cerr << "empty image in " << __FUNCTION__ << std::endl;
    return;
  }

  //check image channel and color type
  if (_src.color() != Type_HSL || _src.channels() != 3) {
    std::cerr << "input color type not hsv in " << __FUNCTION__ << std::endl;
    return;
  }

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  unsigned char *source_data = _src.data();

  unsigned char *result = (unsigned char *) malloc(img_size * 3);

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      unsigned char r, g, b;

      //get the hsl value from source data
      size_t data_loc = i * width + j;
      float h = float(source_data[data_loc]) * 1.4117647f;   //is /255*360
      data_loc += img_size;
      float s = float(source_data[data_loc]) / 255.0f;
      data_loc += img_size;
      float l = float(source_data[data_loc]) / 255.0f;

      float chroma = (1.0f - std::fabsf(2 * l - 1)) * s;
      int sector = std::floor(h / 60.0f);
      float x = chroma * (1.0f - std::fabsf(std::fmodf((h / 60.0f), 2) - 1.0f));

      unsigned char m = (unsigned char) (l - 0.5 * chroma);
      unsigned char c = (unsigned char) chroma;
      unsigned char X = (unsigned char) x;

      switch (sector) {
        case 0:
          r = c + m;
          g = X + m;
          b = m;
          break;

        case 1:
          r = X + m;
          g = c + m;
          b = m;
          break;

        case 2:
          r = m;
          g = c + m;
          b = X + m;
          break;

        case 3:
          r = m;
          g = X + m;
          b = c + m;
          break;

        case 4:
          r = X + m;
          g = m;
          b = c + m;

        default:
          r = c + m;
          g = m;
          b = X + m;
          break;
      }

      data_loc = i * width + j;
      result[data_loc] = r;
      data_loc += img_size;
      result[data_loc] = g;
      data_loc += img_size;
      result[data_loc] = b;
    }
  }

  _dst = u8_image(width, height, 3, Type_RGB, result);
}

void rgb_to_yuv(u8_image &_src, u8_image &_dst) {
  //check image empty
  if (_src.isEmpty()) {
    std::cerr << "empty image in " << __FUNCTION__ << std::endl;
    return;
  }

  //check image type and channels
  if (_src.color() != Type_RGB || _src.channels() != 3) {
    std::cerr << "input color type not rgb in " << __FUNCTION__ << std::endl;
    return;
  }

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  unsigned char *source_data = _src.data();
  unsigned char *result = (unsigned char *) malloc(img_size * 3);

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      size_t data_loc = i * width + j;
      unsigned char r = source_data[data_loc];
      data_loc += img_size;
      unsigned char g = source_data[data_loc];
      data_loc += img_size;
      unsigned char b = source_data[data_loc];

      unsigned char y = 0.299f * r + 0.587f * g + 0.114f * b;
      unsigned char u = -0.14713f * r + -0.28886 * g + 0.436f * b;
      unsigned char v = 0.615f * r + -0.51499f * g + -0.10001f * b;

      data_loc = i * width + j;
      result[data_loc] = y;
      data_loc += img_size;
      result[data_loc] = u;
      data_loc += img_size;
      result[data_loc] = v;
    }
  }

  _dst = u8_image(width, height, 3, Type_YUV444, result);
}

void yuv_to_rgb(u8_image &_src, u8_image &_dst) {
  //check image empty
  if (_src.isEmpty()) {
    std::cerr << "empty image in " << __FUNCTION__ << std::endl;
    return;
  }

  //check image channel and color type
  if (_src.color() != Type_YUV444 || _src.channels() != 3) {
    std::cerr << "input color type not hsv in " << __FUNCTION__ << std::endl;
    return;
  }

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  unsigned char *source_data = _src.data();
  unsigned char *result = (unsigned char *) malloc(img_size * 3);

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      size_t data_loc = i * width + j;
      unsigned char y = source_data[data_loc];
      data_loc += img_size;
      unsigned char u = source_data[data_loc];
      data_loc += img_size;
      unsigned char v = source_data[data_loc];

      unsigned char r = y + 1.13983f * v;
      unsigned char g = y + -0.39465f * u + -0.5806f * v;
      unsigned char b = y + 2.03211f * u;

      data_loc = i * width + j;
      result[data_loc] = r;
      data_loc += img_size;
      result[data_loc] = g;
      data_loc += img_size;
      result[data_loc] = b;
    }
  }

  _dst = u8_image(width, height, 3, Type_RGB, result);
}

void build_gamma_correction_LUT(unsigned char *table, float _gamma_correction) {
  for (size_t i = 0; i < 256; ++i) {
    table[i] = (unsigned char) (255.0f * std::pow((float)i / 255.0f, _gamma_correction));
  }
}

void gamma_correction(u8_image &_src, u8_image &_dst, float _gamma) {
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

  _dst = u8_image(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

void gamma_correction_LUT(u8_image &_src, u8_image &_dst, float _gamma) {
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

  _dst = u8_image(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

void increase_brightness(u8_image &_src,
                         u8_image &_dst,
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

  _dst = u8_image(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

void decrease_brightness(u8_image &_src,
                         u8_image &_dst,
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

  _dst = u8_image(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

void  brightness_adjustment(u8_image &_src,
                           u8_image &_dst,
                           int _adjustment) {
  if (_src.isEmpty()) {
    std::cerr << "source image empty\n";
    _dst = u8_image();
    return;
  }

  if (_adjustment > 128 || _adjustment < -128) {
    std::cerr << "invalid brightness adjustment value\n";
    _dst = u8_image();
    return;
  }

  if (_src.color() != Type_RGB && _src.channels() != 3) {
    std::cerr << "image type must be rgb in " << __FUNCTION__ << "\n";
    return;
  }

  u8_image yuv_img;
  rgb_to_yuv(_src, yuv_img);

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  unsigned char *source_data = yuv_img.data();
}

void contrast_adjustment(u8_image &_src,
                         u8_image &_dst,
                         int _adjustment) {
  if (_src.isEmpty()) {
    std::cerr << "source image empty\n";
    _dst = u8_image();
    return;
  }

  if (_adjustment > 255 || _adjustment < -255) {
    std::cerr << "invalid contrast adjustment value\n";
    _dst = u8_image();
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

  _dst = u8_image(_src.width(), _src.height(), _src.channels(), _src.color(), result);
}

bool white_balance_adjustment(u8_image &_src,
                              u8_image &_dst,
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

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      result[i * width + j] = r_lut[source_data[i * width + j]];
      result[i * width + j + img_size] = g_lut[source_data[i * width + j + img_size]];
      result[i * width + j + img_size + img_size] = b_lut[source_data[i * width + j + img_size + img_size]];
    }
  }

  _dst = qivon::u8_image(width, height, _src.channels(), _src.color(), result);

  return true;
}

bool hue_adjustment(u8_image &_src, u8_image &_dst, int _value) {
  //check source image empty
  if (_src.isEmpty()) {
    std::cerr << "source image empty in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check value out of range
  if (-100 > _value || _value > 100) {
    std::cerr << "value out of range in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check image type
  if (_src.color() != qivon::Type_HSV && _src.color() != qivon::Type_RGB
      && _src.channels() != 3) {
    std::cerr << "image type must be hsv or rgb in " << __FUNCTION__ << std::endl;
    return false;
  }

  u8_image hsv_img;
  if (_src.color() == qivon::Type_HSV)
    hsv_img = _src;
  else {
    //if it's not hsv type, convert it to hsv from rgb
    rgb_to_hsv(_src, hsv_img);
  }

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  unsigned char *source_data = hsv_img.data();
  int hue_change = (int) (float(_value) * 1.28f);

  unsigned char *result = (unsigned char *) malloc(img_size * 3);

  __m128i hue_value = _mm_set1_epi8(hue_change);

  size_t i;
  //use SSE to add the hue chane value to hue
  for (i = 0; i < (img_size - 16); i += 16) {
    __m128i src_vec = _mm_loadu_si128((__m128i*) &source_data[i]);
    __m128i result_vec = _mm_adds_epu8(src_vec, hue_value);

    unsigned char *res = (unsigned char *) &result_vec;

    for (size_t j = 0; j < 16; ++j) {
      result[i + j] = res[j];
    }
  }

  //add the remaining <16 hue
  for (; i < img_size; ++i) {
    result[i] = (unsigned char) std::min(0, std::max(255, (int)source_data[i] + hue_change));
  }

  //copy the saturation and value to the result buffer
  size_t total_img_size = 3 * img_size;
  for (i = img_size; i < total_img_size; ++i) {
    result[i] = source_data[i];
  }

  /*for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      size_t data_loc = i * width + j;
      result[data_loc] = (unsigned char) std::min(0, std::max(255, (int)source_data[data_loc] + hue_change));

      //copy the saturation and value to the result buffer
      data_loc += img_size;
      result[data_loc] = source_data[data_loc];
      data_loc += img_size;
      result[data_loc] = source_data[data_loc];
    }
  }*/

  if (_src.color() == Type_HSV)
    _dst = u8_image(width, height, 3, Type_HSV, result);
  else {
    u8_image hsv_result = u8_image(width, height, 3, Type_HSV, result);
    hsv_to_rgb(hsv_result, _dst);
  }

  return true;
}

bool saturation_adjustment(u8_image &_src, u8_image &_dst, int _value) {
  //check image empty
  if (_src.isEmpty()) {
    std::cerr << "source image empty in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check value out of range
  if (-100 > _value || _value > 100) {
    std::cerr << "value out of range in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check color type and channel
  if (_src.color() != Type_HSV && _src.color() != Type_RGB
      && _src.channels() != 3) {
    std::cerr << "image type must be hsv or rgb in " << __FUNCTION__ << std::endl;
    return false;
  }

  u8_image hsv_img;
  if (_src.color() == Type_HSV)
    hsv_img = _src;
  else
    rgb_to_hsv(_src, hsv_img);

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  unsigned char *source_data = hsv_img.data();
  int saturation_change = (int) ((float)_value * 1.28f);

  //build the lookup table for saturation change
  unsigned char saturation_change_lut[256];
  for (int i = 0; i < 256; ++i) {
    saturation_change_lut[i] = (unsigned char) std::max(0, std::min(255, i + saturation_change));
  }

  unsigned char *result = (unsigned char *) malloc(img_size * 3);

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      size_t data_loc = i * width + j;
      //copy the hue
      result[data_loc] = source_data[data_loc];

      //apply the saturation change
      data_loc += img_size;
      unsigned char val = source_data[data_loc];
      result[data_loc] = saturation_change_lut[val];

      //copy the value
      data_loc += img_size;
      result[data_loc] = source_data[data_loc];
    }
  }

  if (_src.color() == Type_HSV)
    _dst = qivon::u8_image(width, height, 3, Type_HSV, result);
  else {
    u8_image hsv_result = u8_image(width, height, 3, Type_HSV, result);
    hsv_to_rgb(hsv_result, _dst);
  }

  return true;
}
}
