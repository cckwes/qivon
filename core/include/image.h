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
  Image(int width, int height, int channels);
  Image(int width, int height, int channels, T *data);

  //copy constructor
  Image(const Image &img);

  //default destructor
  ~Image();

  bool isEmpty();

  inline int width() const { return m_width; }
  inline int height() const { return m_height; }
  inline int channels() const { return m_channels; }
  inline ColorType color() const {return m_colorType; }
  inline T *data() const { return m_data.get(); }
  inline std::shared_ptr<T> data_ptr() const { return m_data; }

  void setColorType(ColorType _type);

 private:
  int m_width, m_height;
  int m_channels;
  ColorType m_colorType;
  std::shared_ptr<T> m_data;
};

}   //namespace qivon

#endif //QIVON_IMAGE_H
