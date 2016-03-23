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

void toGrayscale(u8_image& src, u8_image& dst) {
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

  unsigned char* const gray = (unsigned char*) malloc(src.width() * src.height());

  if (src.color() == Type_RGB || src.color() == Type_RGBA)
    rgb2Grayscale(src.data(), gray, src.width(), src.height());
  else
    rgb2Grayscale(src.data(), gray, src.width(), src.height(), false);

  dst = u8_image(src.width(), src.height(), 1, Type_Grayscale, gray);
}

void rgbSwap(u8_image& src, u8_image& dst) {
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

  unsigned char* const result = (unsigned char*) malloc(3 * kImgSize);
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

void rgb2Hsv(u8_image& src, u8_image& dst) {
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

  unsigned char* const result = (unsigned char*) malloc(kImgSize * 3);

  for (size_t i = 0; i < kHeight; ++i) {
    for (size_t j = 0; j < kWidth; ++j) {
      const size_t pixel_position = i * kWidth + j;
      float r = float(original[pixel_position]) / 255.0f;
      float g = float(original[pixel_position + kImgSize]) / 255.0f;
      float b = float(original[pixel_position + 2 * kImgSize]) / 255.0f;

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
      result[pixel_position] = h;
      result[pixel_position + kImgSize] = s;
      result[pixel_position + 2 * kImgSize] = v;
    }
  }

  dst = qivon::u8_image(kWidth, kHeight, 3, qivon::Type_HSV, result);
}

void hsv2Rgb(u8_image& src, u8_image& dst) {
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

  unsigned char* const result = (unsigned char*) malloc(kImgSize * 3);

  for (size_t i = 0; i < kHeight; ++i) {
    for (size_t j = 0; j < kWidth; ++j) {
      const size_t pixel_position = i * kWidth + j;
      float h = float(original[pixel_position]) * 1.4117647f;   //is /255*360
      float s = float(original[pixel_position + kImgSize]) / 255.0f;
      float v = float(original[pixel_position + 2 * kImgSize]) / 255.0f;

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

      result[pixel_position] = r;
      result[pixel_position + kImgSize] = g;
      result[pixel_position + 2 * kImgSize] = b;
    }
  }

  dst = u8_image(kWidth, kHeight, 3, Type_RGB, result);
}

void rgb2Hsl(u8_image& src, u8_image& dst) {
  //check image empty
  if (src.isEmpty()) {
    std::cerr << "Source image empty in " << __FUNCTION__ << std::endl;
    return;
  }

  //check image type and channels
  if (src.color() != Type_RGB || src.channels() != 3) {
    std::cerr << "input color type not rgb in " << __FUNCTION__ << std::endl;
    return;
  }

  const size_t& kWidth = src.width();
  const size_t& kHeight = src.height();
  const size_t kImgSize = kWidth * kHeight;
  const unsigned char* const original = src.data();
  unsigned char* const result = (unsigned char*) malloc(kImgSize * 3);

  for (size_t i = 0; i < kHeight; ++i) {
    for (size_t j = 0; j < kWidth; ++j) {
      const size_t pixel_position = i * kWidth + j;
      float r = float(original[pixel_position]) / 255.0f;
      float g = float(original[pixel_position + kImgSize]) / 255.0f;
      float b = float(original[pixel_position + 2 * kImgSize]) / 255.0f;

      float min = std::min(r, std::min(g, b));
      float max = std::max(r, std::max(g, b));
      float delta = max - min;

      //get the L value
      unsigned char l = (unsigned char) ((max + min) / 2.0f * 255.0f);

      //get the S value
      unsigned char s = (delta == 0) ? 0 : (unsigned char) (delta / 1.0 - std::fabsf(max + min - 1));

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
      size_t data_loc = pixel_position;
      result[data_loc] = h;
      data_loc += kImgSize;
      result[data_loc] = s;
      data_loc += kImgSize;
      result[data_loc] = l;
    }
  }

  dst = u8_image(kWidth, kHeight, 3, Type_HSL, result);
}

void hsl2Rgb(u8_image& src, u8_image& dst) {
  //check image empty
  if (src.isEmpty()) {
    std::cerr << "Source image empty in " << __FUNCTION__ << std::endl;
    return;
  }

  //check image channel and color type
  if (src.color() != Type_HSL || src.channels() != 3) {
    std::cerr << "input color type not hsv in " << __FUNCTION__ << std::endl;
    return;
  }

  const size_t& kWidth = src.width();
  const size_t& kHeight = src.height();
  const size_t kImgSize = kWidth * kHeight;
  const unsigned char* const original = src.data();

  unsigned char* const result = (unsigned char *) malloc(kImgSize * 3);

  for (size_t i = 0; i < kHeight; ++i) {
    for (size_t j = 0; j < kWidth; ++j) {
      unsigned char r, g, b;

      //get the hsl value from source data
      size_t data_loc = i * kWidth + j;
      float h = float(original[data_loc]) * 1.4117647f;   //is /255*360
      data_loc += kImgSize;
      float s = float(original[data_loc]) / 255.0f;
      data_loc += kImgSize;
      float l = float(original[data_loc]) / 255.0f;

      const float chroma = (1.0f - std::fabsf(2 * l - 1)) * s;
      const int sector = std::floor(h / 60.0f);
      const float x = chroma * (1.0f - std::fabsf(std::fmodf((h / 60.0f), 2) - 1.0f));

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

      data_loc = i * kWidth + j;
      result[data_loc] = r;
      data_loc += kImgSize;
      result[data_loc] = g;
      data_loc += kImgSize;
      result[data_loc] = b;
    }
  }

  dst = u8_image(kWidth, kHeight, 3, Type_RGB, result);
}

void rgb2Yuv(u8_image& src, u8_image& dst) {
  //check image empty
  if (src.isEmpty()) {
    std::cerr << "Source image empty in " << __FUNCTION__ << std::endl;
    return;
  }

  //check image type and channels
  if (src.color() != Type_RGB || src.channels() != 3) {
    std::cerr << "input color type not rgb in " << __FUNCTION__ << std::endl;
    return;
  }

  static bool lut_init = false;
  static std::vector<float> container;
  static const int kNumCoefficients = 9;
  static const float coefficients[kNumCoefficients] = {0.257f, -0.148f, 0.439f,
                                                       0.504f, -0.291f, -0.368f,
                                                       0.098f, 0.439f, -0.071f};

  if (!lut_init) {
    container.clear();
    container.resize(kNumCoefficients * 256);

    //initialize the LUT values
    for (int i = 0; i < 256; ++i) {
      for (int j = 0; j < kNumCoefficients; ++j) {
        container[i + j * 256] = coefficients[j] * i;
      }
    }
    lut_init = true;
  }

  const size_t& kWidth = src.width();
  const size_t& kHeight = src.height();
  const size_t kImgSize = kWidth * kHeight;
  const unsigned char* const original = src.data();
  unsigned char* const result = (unsigned char *) malloc(kImgSize * 3);

  for (size_t i = 0; i < kHeight; ++i) {
    for (size_t j = 0; j < kWidth; ++j) {
      size_t data_loc = i * kWidth + j;
      unsigned char r = original[data_loc];
      data_loc += kImgSize;
      unsigned char g = original[data_loc];
      data_loc += kImgSize;
      unsigned char b = original[data_loc];

      unsigned char y = std::max(16, std::min(235, (int)
          (container[r] + container[g + 3 * 256] + container[b + 6 * 256] + 16)));
      unsigned char u = std::max(16, std::min(240, (int)
          (container[r + 256] + container[g + 4 * 256] + container[b + 7 * 256] + 128)));
      unsigned char v = std::max(16, std::min(240, (int)
          (container[r + 2 * 256] + container[g + 5 * 256] + container[b + 8 * 256] + 128)));

      data_loc = i * kWidth + j;
      result[data_loc] = y;
      data_loc += kImgSize;
      result[data_loc] = u;
      data_loc += kImgSize;
      result[data_loc] = v;
    }
  }

  dst = u8_image(kWidth, kHeight, 3, Type_YUV444, result);
}

void yuv2Rgb(u8_image& src, u8_image& dst) {
  //check image empty
  if (src.isEmpty()) {
    std::cerr << "Source image empty in " << __FUNCTION__ << std::endl;
    return;
  }

  //check image channel and color type
  if (src.color() != Type_YUV444 || src.channels() != 3) {
    std::cerr << "input color type not hsv in " << __FUNCTION__ << std::endl;
    return;
  }

  static bool lut_init = false;
  static std::vector<float> container;
  static const int kNumCoefficients = 5;
  static const float coefficients[kNumCoefficients] = {1.164f, 1.596f, -0.391f,
                                                       -0.813f, 2.018f};

  if (!lut_init) {
    container.clear();
    container.resize(kNumCoefficients * 256);

    //initialize the LUT values
    for (int i = 0; i < 256; ++i) {
      for (int j = 0; j < kNumCoefficients; ++j) {
        float res_val;

        if (j == 0) {
          if (i <= 16)
            res_val = 0;
          else
            res_val = ((i - 16) * coefficients[j]);
        } else {
          if (i <= 128)
            res_val = 0;
          else
            res_val = ((i - 128) * coefficients[j]);
        }

        container[i + j * 256] = res_val;
      }
    }
    lut_init = true;
  }

  const size_t& kWidth = src.width();
  const size_t& kHeight = src.height();
  const size_t kImgSize = kWidth * kHeight;
  const unsigned char* const original = src.data();
  unsigned char* const result = (unsigned char*) malloc(kImgSize * 3);

  for (size_t i = 0; i < kHeight; ++i) {
    for (size_t j = 0; j < kWidth; ++j) {
      size_t data_loc = i * kWidth + j;
      unsigned char y = original[data_loc];
      data_loc += kImgSize;
      unsigned char u = original[data_loc];
      data_loc += kImgSize;
      unsigned char v = original[data_loc];

      unsigned char r = std::max(0,std::min(255, (int)
          (container[y] + container[v + 256])));
      unsigned char g = std::max(0,std::min(255, (int)
          (container[y] + container[u + 2 * 256] + container[v + 3 * 256])));
      unsigned char b = std::max(0,std::min(255, (int)
          (container[y] + container[u + 4 * 256])));

      data_loc = i * kWidth + j;
      result[data_loc] = r;
      data_loc += kImgSize;
      result[data_loc] = g;
      data_loc += kImgSize;
      result[data_loc] = b;
    }
  }

  dst = u8_image(kWidth, kHeight, 3, Type_RGB, result);
}

void buildGammaCorrectionLUT(unsigned char *table, const float gamma_correction) {
  for (size_t i = 0; i < 256; ++i) {
    table[i] = (unsigned char) (255.0f * std::pow((float)i / 255.0f, gamma_correction));
  }
}

void gammaCorrection(u8_image& src, u8_image& dst, float _gamma) {
  if (src.isEmpty()) {
    std::cerr << "source image empty\n";
    return;
  }

  float gamma_correction = 1.0f / _gamma;

  unsigned char *result = (unsigned char *) malloc(src.channels() * src.width()
                                                       * src.height());

  size_t image_size = src.width() * src.height() * src.channels();

  for (size_t i = 0; i < image_size; ++i) {
    result[i] = (unsigned char) (255.0f * std::pow(src.data()[i] / 255.0f, gamma_correction));
  }

  dst = u8_image(src.width(), src.height(), src.channels(), src.color(), result);
}

void gammaCorrectionLUT(u8_image& src, u8_image& dst, float _gamma) {
  if (src.isEmpty()) {
    std::cerr << "source image empty\n";
    return;
  }

  const float kGammaCorrection = 1.0f / _gamma;

  const size_t kImgSize = src.width() * src.height() * src.channels();

  unsigned char table[256];

  buildGammaCorrectionLUT(table, kGammaCorrection);

  const unsigned char* const original = src.data();
  unsigned char* const result = (unsigned char*) malloc(kImgSize);

  for (size_t i = 0; i < kImgSize; ++i) {
    result[i] = table[original[i]];
  }

  dst = u8_image(src.width(), src.height(), src.channels(), src.color(), result);
}

void increaseBrightness(u8_image& src,
                         u8_image& dst,
                         unsigned char _value) {
  const size_t image_size = src.width() * src.height() * src.channels();

  unsigned char *result = (unsigned char *) malloc(image_size);

  __m128i value = _mm_set1_epi8(_value);

  size_t i;
  for (i = 0; i < (image_size - 16); i += 16) {
    __m128i src_vec = _mm_loadu_si128((__m128i*) &src.data()[i]);
    __m128i result_vec = _mm_adds_epu8(src_vec, value);

    unsigned char *res = (unsigned char *) &result_vec;

    for (size_t j = 0; j < 16; ++j) {
      result[i + j] = res[j];
    }
  }

  for (; i < image_size; ++i) {
    result[i] = src.data()[i] + _value;
  }

  dst = u8_image(src.width(), src.height(), src.channels(), src.color(), result);
}

void decreaseBrightness(u8_image& src,
                         u8_image& dst,
                         unsigned char _value) {
  const size_t image_size = src.width() * src.height() * src.channels();

  unsigned char *result = (unsigned char *) malloc(image_size);

  __m128i value = _mm_set1_epi8(_value);

  size_t i;
  for (i = 0; i < (image_size - 16); i += 16) {
    __m128i src_vec = _mm_loadu_si128((__m128i*) &src.data()[i]);
    __m128i result_vec = _mm_subs_epu8(src_vec, value);

    unsigned char *res = (unsigned char *) &result_vec;

    for (size_t j = 0; j < 16; ++j) {
      result[i + j] = res[j];
    }
  }

  for (; i < image_size; ++i) {
    result[i] = src.data()[i] - _value;
  }

  dst = u8_image(src.width(), src.height(), src.channels(), src.color(), result);
}

void  brightnessAdjustment(u8_image& src,
                           u8_image& dst,
                           int _adjustment) {
  if (src.isEmpty()) {
    std::cerr << "source image empty\n";
    dst = u8_image();
    return;
  }

  if (_adjustment > 128 || _adjustment < -128) {
    std::cerr << "invalid brightness adjustment value\n";
    dst = u8_image();
    return;
  }

  if (src.color() != Type_RGB && src.channels() != 3) {
    std::cerr << "image type must be rgb in " << __FUNCTION__ << "\n";
    return;
  }

  u8_image yuv_img;
  rgb2Yuv(src, yuv_img);

  size_t width = src.width();
  size_t height = src.height();
  size_t img_size = width * height;
  unsigned char *source_data = yuv_img.data();
}

void contrastAdjustment(u8_image& src,
                         u8_image& dst,
                         int _adjustment) {
  if (src.isEmpty()) {
    std::cerr << "source image empty\n";
    dst = u8_image();
    return;
  }

  if (_adjustment > 255 || _adjustment < -255) {
    std::cerr << "invalid contrast adjustment value\n";
    dst = u8_image();
    return;
  }

  unsigned char lut[256];

  float factor = 259.0f * float(_adjustment + 255) / (255.0f * float(259 - _adjustment));

  //build the LUT
  for (size_t i = 0; i < 256; ++i) {
    lut[i] = (unsigned char) std::max(std::min((int) (factor * (i - 128) + 128), 255), 0);
  }

  size_t image_size = src.width() * src.height() * src.channels();

  unsigned char *result = (unsigned char *) malloc(image_size);
  unsigned char *source = src.data();

  for (size_t j = 0; j < image_size; ++j) {
    result[j] = lut[source[j]];
  }

  dst = u8_image(src.width(), src.height(), src.channels(), src.color(), result);
}

bool whiteBalanceAdjustment(u8_image& src,
                              u8_image& dst,
                              int temperature) {
  //check temperature out of range
  if (temperature < 1000 || temperature > 20000) {
    std::cerr << "temperature out of range in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check image empty
  if (src.isEmpty()) {
    std::cerr << "source image empty in " << __FUNCTION__ << std::endl;
    return false;
  }

  //image must be RGB type
  if (src.color() != qivon::Type_RGB) {
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

  size_t width = src.width();
  size_t height = src.height();
  size_t img_size = width * height;
  unsigned char *source_data = src.data();

  unsigned char *result = (unsigned char*) malloc(img_size * src.channels());

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      result[i * width + j] = r_lut[source_data[i * width + j]];
      result[i * width + j + img_size] = g_lut[source_data[i * width + j + img_size]];
      result[i * width + j + img_size + img_size] = b_lut[source_data[i * width + j + img_size + img_size]];
    }
  }

  dst = qivon::u8_image(width, height, src.channels(), src.color(), result);

  return true;
}

bool hueAdjustment(u8_image& src, u8_image& dst, int _value) {
  //check source image empty
  if (src.isEmpty()) {
    std::cerr << "source image empty in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check value out of range
  if (-100 > _value || _value > 100) {
    std::cerr << "value out of range in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check image type
  if (src.color() != qivon::Type_HSV && src.color() != qivon::Type_RGB
      && src.channels() != 3) {
    std::cerr << "image type must be hsv or rgb in " << __FUNCTION__ << std::endl;
    return false;
  }

  u8_image hsv_img;
  if (src.color() == qivon::Type_HSV)
    hsv_img = src;
  else {
    //if it's not hsv type, convert it to hsv from rgb
    rgb2Hsv(src, hsv_img);
  }

  size_t width = src.width();
  size_t height = src.height();
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

  if (src.color() == Type_HSV)
    dst = u8_image(width, height, 3, Type_HSV, result);
  else {
    u8_image hsv_result = u8_image(width, height, 3, Type_HSV, result);
    hsv2Rgb(hsv_result, dst);
  }

  return true;
}

bool saturationAdjustment(u8_image& src, u8_image& dst, int _value) {
  //check image empty
  if (src.isEmpty()) {
    std::cerr << "source image empty in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check value out of range
  if (-100 > _value || _value > 100) {
    std::cerr << "value out of range in " << __FUNCTION__ << std::endl;
    return false;
  }

  //check color type and channel
  if (src.color() != Type_HSV && src.color() != Type_RGB
      && src.channels() != 3) {
    std::cerr << "image type must be hsv or rgb in " << __FUNCTION__ << std::endl;
    return false;
  }

  u8_image hsv_img;
  if (src.color() == Type_HSV)
    hsv_img = src;
  else
    rgb2Hsv(src, hsv_img);

  size_t width = src.width();
  size_t height = src.height();
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

  if (src.color() == Type_HSV)
    dst = qivon::u8_image(width, height, 3, Type_HSV, result);
  else {
    u8_image hsv_result = u8_image(width, height, 3, Type_HSV, result);
    hsv2Rgb(hsv_result, dst);
  }

  return true;
}
}
