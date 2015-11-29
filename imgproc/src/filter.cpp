#if defined(__GNUC__)
#include <x86intrin.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#else
#error Unknown compiler, not including any intrinsic header
#endif

#include <iostream>
#include "filter.h"

namespace qivon {

void meanFilter(Image<unsigned char> &_src, Image<unsigned char> &_dst) {
}

}   //namespace qivon