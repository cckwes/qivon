#include <stdlib.h>
#include <image.h>
#include <iostream>

namespace qivon {
template<class T>
Image<T>::Image()
    : m_width(0), m_height(0), m_channels(0), m_colorType(Type_Invalid) {
}

template<class T>
Image<T>::Image(size_t width, size_t height, size_t channels)
    : m_width(width), m_height(height), m_channels(channels), m_colorType(Type_Invalid) {
  if (width == 0 || height == 0 || channels == 0)
    return;

  T *data = (T*) malloc(m_width * m_height * m_channels * sizeof(T));
  m_data.reset(data);
}

template<class T>
Image<T>::Image(size_t width, size_t height, size_t channels, T *data)
    : m_width(width), m_height(height), m_channels(channels), m_colorType(Type_Invalid) {
  m_data.reset(data);
}

template<class T>
Image<T>::Image(const Image<T> &img) {
  m_width = img.width();
  m_height = img.height();
  m_channels = img.channels();
  m_colorType = img.color();

  //shallow copy
  m_data = img.data_ptr();
}

template <class T>
Image<T>::Image(size_t width, size_t height, size_t channels, ColorType type)
    : m_width(width), m_height(height), m_channels(channels), m_colorType(type) {
  if (width == 0 || height == 0 || channels == 0)
    return;

  T *data = (T*) malloc(m_width * m_height * m_channels * sizeof(T));
  m_data.reset(data);
}

template <class T>
Image<T>::Image(size_t width, size_t height, size_t channels, ColorType type, T *data)
    : m_width(width), m_height(height), m_channels(channels), m_colorType(type) {
  m_data.reset(data);
}

template<class T>
Image<T>::~Image() {
}

template<class T>
bool Image<T>::isEmpty() {
  return (m_width == 0 && m_height == 0) || m_data.get() == nullptr;
}

template <class T>
void Image<T>::setColorType(ColorType _type) {
  m_colorType = _type;
}

template <class T>
std::vector<T> Image<T>::get(size_t u, size_t v) {
  std::vector<T> result;

  if (isEmpty())
    return result;

  if (u >= m_width || v >= m_height) {
    std::cerr << "u or v out of range with " << u << "," << v << "\n";
    return result;
  }

  size_t image_size = m_width * m_height;

  T* ptr = m_data.get();

  //push back the grayscale value
  //or 1st channel for 3/4 channels image
  result.push_back(ptr[v * m_width + u]);

  if (m_channels == 3) {
    result.push_back(ptr[v * m_height + u + image_size]);
    result.push_back(ptr[v * m_height + u + 2 * image_size]);
  } else if (m_channels == 4) {
    result.push_back(ptr[v * m_height + u + image_size]);
    result.push_back(ptr[v * m_height + u + 2 * image_size]);
    result.push_back(ptr[v * m_height + u + 3 * image_size]);
  }

  return result;
}

//deep copy
template <class T>
Image<T> Image<T>::deepCopy() {
  size_t image_size = m_width * m_height * m_channels;
  T *result = (T*) malloc(image_size * sizeof(T));

  memcpy(result, m_data.get(), image_size * sizeof(T));

  return qivon::Image<T>(m_width, m_height, m_channels, m_colorType, result);
}

//8 bit image
template
class Image<unsigned char>;

//16 bit image
template
class Image<unsigned short>;

//32 bit float image
template
class Image<float>;

}   //namespace qivon