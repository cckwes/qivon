#ifndef QIVON_FILTER_H
#define QIVON_FILTER_H

#include <qivon_config.h>
#include <image.h>

namespace qivon {

QIVON_EXPORT void meanFilter(Image<unsigned char> &_src, Image<unsigned char> &_dst);

}   //namespace qivon

#endif  //QIVON_FILTER_H