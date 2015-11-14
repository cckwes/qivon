#ifndef QIVON_QIVON_CONFIG_H
#define QIVON_QIVON_CONFIG_H

#if defined(_MSC_VER)
    #ifdef QIVON_LIBRARY
        #define QIVON_EXPORT __declspec(dllexport)
    #else
        #define QIVON_EXPORT __declspec(dllimport)
    #endif
#else
    #ifdef QIVON_LIBRARY
        #define QIVON_EXPORT __attribute__((visibility("default")))
    #else
        #define QIVON_EXPORT __attribute__((visibility("default")))
    #endif
#endif

#endif //QIVON_QIVON_CONFIG_H
