#include <iostream>
#include <cmath>
#include "util.h"
#include "geometry.h"

namespace qivon {

template <class T>
T cubicInterpolate(T p[4], float x) {
  return p[1] + 0.5 * x * (p[2] - p[0] + x * (2.0 * p[0] - 5.0 * p[1]
      + 4.0 * p[2] - p[3] + x * (3.0 * (p[1] - p[2]) + p[3] - p[0])));
}

template <class T>
T bicubicInterpolate(T p[4][4], float x, float y) {
  T arr[4];
  arr[0] = cubicInterpolate(p[0], y);
  arr[1] = cubicInterpolate(p[1], y);
  arr[2] = cubicInterpolate(p[2], y);
  arr[3] = cubicInterpolate(p[3], y);
  return cubicInterpolate(arr, x);
}

//////////////////////////////
// resize image
//////////////////////////////
template <class T>
void resizeImage(Image<T> &_src, Image<T> &_dst,
                 size_t _width, size_t _height,
                 bool _keep_aspect_ratio) {
  if (_src.isEmpty()) {
    std::cerr << "soure image empty in " << __FUNCTION__
        << " at " << __LINE__ << std::endl;
    return;
  }

  //get all the parameters ready
  size_t original_width = _src.width();
  size_t original_height = _src.height();
  size_t original_image_size = original_width * original_height;
  size_t destination_image_size = _width * _height;
  size_t channels = _src.channels();
  T *original_data = _src.data();

  //calculate the width and height ratio
  float ratio_width = float(_src.width()) / _width;
  float ratio_height = float(_src.height()) / _height;

  //ratio correction if keep aspect ratio option is enabled
  if (_keep_aspect_ratio) {
    float ratio = std::min(ratio_width, ratio_height);
    ratio_width = ratio_height = ratio;

    //correct the width and heigh of destination image
    _width = std::floor(float(_src.width()) / ratio_width);
    _height = std::floor(float(_src.height()) / ratio_height);
    destination_image_size = _width * _height;
  }

  //malloc the result pointer after
  //width and height of destination image is corrected
  T *result = (T*) malloc(_width * _height * channels * sizeof(T));

  //loop through every destination pixel
  for (size_t i = 0; i < _height; ++i) {
    float v = i * ratio_height;
    for (size_t j = 0; j < _width; ++j) {
      float u = j * ratio_width;

      size_t u_discrete = std::floor(u);
      size_t v_discrete = std::floor(v);

      size_t start_x, start_y, end_x, end_y;

      start_x = (u_discrete < 2) ? 0 : (u_discrete > (_src.width() - 3)) ? _src.width() - 4 : u_discrete - 1;
      start_y = (v_discrete < 2) ? 0 : (v_discrete > (_src.height() - 3)) ? _src.height() - 4 : v_discrete - 1;

      end_x = (u_discrete > (_src.width() - 3)) ? _src.width() - 1 : (u_discrete < 2) ? 3 : u_discrete + 2;
      end_y = (v_discrete > (_src.height() - 3)) ? _src.height() - 1 : (v_discrete < 2) ? 3 : v_discrete + 2;

      float pos_x, pos_y;

      //find the x position
      if (u_discrete < 2)   //left size
        pos_x = u;
      else if (u_discrete > (_src.width() - 3))   //right side
        pos_x = 4.0f - (_src.width() - u);
      else
        pos_x = u - float(u_discrete) + 1.0f;

      //find the y position
      if (v_discrete < 2)
        pos_y = v;
      else if (v_discrete > (_src.height() - 3))
        pos_y = 4.0f - (_src.height() - v);
      else
        pos_y = v - float(v_discrete) + 1.0f;

      //for each channel
      for (size_t ch = 0; ch < channels; ++ch) {

        T data[4][4];

        //get the data
        for (size_t k = start_y; k <= end_y; ++k) {
          for (size_t l = start_x; l <= end_x; ++l) {
            data[k - start_y][l - start_x] =
                original_data[k * original_width + l + ch * original_image_size];
          }
        }

        T pt_result = bicubicInterpolate(data, pos_x, pos_y);

        result[i * _width + j + ch * destination_image_size] = pt_result;
      }
    }
  }

  _dst = qivon::Image<T>(_width, _height, channels, _src.color(), result);
}

template
void resizeImage<unsigned char>(Image<unsigned char> &_src, Image<unsigned char> &_dst,
                                            size_t _width, size_t _height, bool _keep_aspect_ratio);

template
void resizeImage<unsigned short>(Image<unsigned short> &_src, Image<unsigned short> &_dst,
                                 size_t width, size_t _height, bool _keep_aspect_ratio);

template
void resizeImage<float>(Image<float> &_src, Image<float> &_dst,
                        size_t _width, size_t _height, bool _keep_aspect_ratio);
//////////////////////////////


//////////////////////////////
// crop image
//////////////////////////////
template <class T>
void cropImage(Image<T> &_src, Image<T> &_dst,
               size_t _start_x, size_t _start_y,
               size_t _end_x, size_t _end_y) {
  //check image empty
  if (_src.isEmpty()) {
    std::cerr << "soure image empty in " << __FUNCTION__
        << " at " << __LINE__ << std::endl;
    return;
  }

  if (_start_x >= _end_x || _start_y >= _end_y) {
    std::cerr << "logic error in " << __FUNCTION__ << std::endl;
    return;
  }

  size_t width = _src.width();
  size_t height = _src.height();
  size_t img_size = width * height;
  size_t origin = 0;

  //check start and end x,y out of bound
  if (!isWithin(_start_x, origin, width - 1) || !isWithin(_end_x, origin, width - 1)
      || !isWithin(_start_y, origin, height - 1) || !isWithin(_end_y, origin, height - 1)) {
    std::cerr << "crop size out of range in " << __FUNCTION__ << std::endl;
    return;
  }

  size_t dst_width = _end_x - _start_x;
  size_t dst_height = _end_y - _start_y;
  size_t dst_img_size = dst_height * dst_width;
  size_t channels = _src.channels();
  T* result = (T *) malloc(dst_height * dst_width * channels * sizeof(T));

  size_t y = 0;
  for (size_t i = _start_y; i < _end_y; ++i) {
    for (size_t ch = 0; ch < channels; ++ch) {
      std::copy(_src.data() + i * width + _start_x + ch * img_size,
                _src.data() + i * width + _end_x + ch * img_size,
                result + y * dst_width + ch * dst_img_size);
    }
    ++y;
  }

  _dst = qivon::Image<T>(dst_width, dst_height, channels, _src.color(), result);
}


template void cropImage<unsigned char>(Image<unsigned char> &_src, Image<unsigned char> &_dst,
                                       size_t _start_x, size_t _start_y,
                                       size_t _end_x, size_t _end_y);

template void cropImage<unsigned short>(Image<unsigned short> &_src, Image<unsigned short> &_dst,
                                       size_t _start_x, size_t _start_y,
                                       size_t _end_x, size_t _end_y);

template void cropImage<float>(Image<float> &_src, Image<float> &_dst,
                               size_t _start_x, size_t _start_y,
                               size_t _end_x, size_t _end_y);
//////////////////////////////

} //namespace qivon
