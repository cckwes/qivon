#ifndef QIVON_IMAGE_H
#define QIVON_IMAGE_H

#include <memory>

#include "qivon_config.h"

namespace qivon {

enum ColorType {
  Type_Invalid = -1,
  Type_Grayscale = 0,
  Type_RGB = 1,
  Type_BGR = 2,
  Type_RGBA = 3
};

template<class T>
class QIVON_EXPORT Image {
 public:
  Image();
  Image(size_t width, size_t  height, size_t channels);
  Image(size_t width, size_t height, size_t channels, T *data);
  Image(size_t width, size_t height, size_t channels, ColorType type);
  Image(size_t width, size_t height, size_t channels, ColorType type, T *data);

  //copy constructor
  Image(const Image &img);

  //default destructor
  ~Image();

  bool isEmpty();

  inline size_t width() const { return m_width; }
  inline size_t height() const { return m_height; }
  inline size_t channels() const { return m_channels; }
  inline ColorType color() const {return m_colorType; }
  inline T *data() const { return m_data.get(); }
  inline std::shared_ptr<T> data_ptr() const { return m_data; }

  void setColorType(ColorType _type);

 private:
  size_t m_width, m_height;
  size_t m_channels;
  ColorType m_colorType;
  std::shared_ptr<T> m_data;
};

}   //namespace qivon

#endif //QIVON_IMAGE_H
