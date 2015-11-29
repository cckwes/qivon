#if defined(__GNUC__)
#include <x86intrin.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#else
#error Unknown compiler, not including any intrinsic header
#endif

#include <iostream>
#include <vector>
#include <chrono>
#include "filter.h"

namespace qivon {

void meanFilter3x3_ch1(Image<unsigned char> &_src,
                       Image<unsigned char> &_dst) {
  size_t width = _src.width();
  size_t height = _src.height();
  size_t image_size = width * height;

  unsigned char *result = (unsigned char *) malloc(image_size * sizeof(unsigned char));

  for (int i = 0; i < height; ++i) {
    int previous_row = i - 1 < 0 ? 1 : i - 1;
    int next_row = i + 1 >= height ? height - 2 : i + 1;

    //unroll the loop to add all elements
    int total = (int) _src.get(0, previous_row)[0]
        + 2 * (int) _src.get(1, previous_row)[0] + (int) _src.get(0, i)[0]
        + 2 * (int) _src.get(1, i)[0] + (int) _src.get(0, next_row)[0]
        + 2 * (int) _src.get(1, next_row)[0];

    //process the 0th element of each row
    result[i * width] = (unsigned char) (total / 9);

    for (int j = 1; j < width; ++j) {
      //identify the addition or subtraction column index
      int subtract_column = (j - 2) < 0 ? 1 : j - 2;
      int add_column = (j + 1) >= width ? width - 2 : j + 1;

      int subtract_total = (int) _src.get(subtract_column, previous_row)[0]
          + (int) _src.get(subtract_column, i)[0]
          + (int) _src.get(subtract_column, next_row)[0];

      int add_total = (int) _src.get(add_column, previous_row)[0]
          + (int) _src.get(add_column, i)[0]
          + (int) _src.get(add_column, next_row)[0];

      total = total + add_total - subtract_total;

      result[i * width + j] = (unsigned char) (total / 9);
    }
  }

  _dst = Image<unsigned char>(width, height, 1, Type_Grayscale, result);
}

void meanFilter3x3_ch1_separable(Image<unsigned char> &_src,
                                 Image<unsigned char> &_dst) {
  size_t width = _src.width();
  size_t height = _src.height();
  size_t image_size = width * height;

  unsigned char *temporary = (unsigned char *) malloc(image_size * sizeof(unsigned char));
  unsigned char *result = (unsigned char *) malloc(image_size * sizeof(unsigned char));

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int previous_u = j - 1 >= 0 ? j - 1 : 1;
      int next_u = j + 1 < width ? j + 1 : width - 2;

      temporary[i * width + j] = (unsigned char) ((_src.data()[i * width + previous_u]
          + _src.data()[i * width + j] + _src.data()[i * width + next_u]) / 3);
    }
  }

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int previous_v = i - 1 >= 0 ? i - 1 : 1;
      int next_v = i + 1 < height ? i + 1: height - 2;

      result[i * width + j] = (unsigned char) ((temporary[previous_v * width + j]
          + temporary[i * width + j] + temporary[next_v * width + j]) / 3);
    }
  }

  _dst = Image<unsigned char>(width, height, 1, Type_Grayscale, result);
}

void meanFilter3x3(Image<unsigned char> &_src,
                Image<unsigned char> &_dst) {
  if (_src.isEmpty()
      || _src.width() <= 3
      || _src.height() <= 3) {
    std::cerr << "empty sourc image or invalid size in "
        << __FUNCTION__ << " at line " << __LINE__ << "\n";
    return;
  }

  if (_src.channels() == 1) {
    meanFilter3x3_ch1_separable(_src, _dst);
  }
}

}   //namespace qivon