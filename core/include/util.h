#ifndef QIVON_UTIL_H
#define QIVON_UTIL_H

namespace qivon {

template <class T>
bool isWithin(T _check_number, T _start, T _end) {
  return _check_number >= _start && _check_number <= _end;
}

}

#endif //QIVON_UTIL_H
