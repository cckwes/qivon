#include <image.h>

namespace qivon {
template<class T>
Image<T>::Image()
    : m_width(0), m_height(0), m_channels(0) {
}

template<class T>
Image<T>::Image(int width, int height, int channels)
    : m_width(0), m_height(0), m_channels(0) {
  m_width = width > 0 ? width : 0;
  m_height = height > 0 ? height : 0;
  m_channels = channels > 0 ? channels : 0;
}

template<class T>
Image<T>::Image(int width, int height, int channels, T *data)
    : m_width(0), m_height(0), m_channels(0) {
  m_width = width > 0 ? width : 0;
  m_height = height > 0 ? height : 0;
  m_channels = channels > 0 ? channels : 0;
  m_data.reset(data);
}

template<class T>
Image<T>::Image(const Image<T> &img) {
  m_width = img.width();
  m_height = img.height();
  m_channels = img.channels();

  //shallow copy
  m_data = img.data_ptr();
}

template<class T>
Image<T>::~Image() {
}

template<class T>
bool Image<T>::isEmpty() {
  return m_width == 0 && m_height == 0 || m_data.get() == nullptr;

}

template
class Image<unsigned char>;

template
class Image<float>;
}   //namespace qivon