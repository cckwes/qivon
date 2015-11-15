#ifndef QIVON_IMAGE_H
#define QIVON_IMAGE_H

#include <memory>

#include "qivon_config.h"

namespace qivon {

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
  inline T *data() const { return m_data.get(); }
  inline std::shared_ptr<T> data_ptr() const { return m_data; }

 private:
  int m_width, m_height;
  int m_channels;
  std::shared_ptr<T> m_data;
};

}   //namespace qivon

#endif //QIVON_IMAGE_H
