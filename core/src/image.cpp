#include <image.h>

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