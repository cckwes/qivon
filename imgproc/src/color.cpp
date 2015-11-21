#if defined(__GNUC__)
#include <x86intrin.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#else
#error Unknown compiler, not including any intrinsic header
#endif

#include "color.h"

namespace qivon {

void toGrayscale(Image<unsigned char> &_src, Image<unsigned char> &_dst)
{
}

}