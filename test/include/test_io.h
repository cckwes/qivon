#ifndef TEST_IO_H
#define TEST_IO_H

#include <string>

bool test_read_png(const std::string &_filename);

bool test_read_jpg(const std::string &_filename);

bool test_write_png(const std::string &_filename, qivon::Image<unsigned char> &_img);

bool test_write_jpg(const std::string &_filename, qivon::Image<unsigned char> &_img,
                    int _quality);

#endif    //TEST_IO_H