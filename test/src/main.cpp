#include <iostream>
#include <pngio.h>
#include <jpgio.h>
#include <color.h>

#include "test_io.h"
#include "test_geometry.h"
#include "test_filter.h"
#include "test_color.h"

int g_test_count = 0;
int g_test_failed = 0;
int g_test_succeed = 0;

void update_test_counter(bool succeed) {
  ++g_test_count;

  if (succeed)
    ++g_test_succeed;
  else
    ++g_test_failed;
}

void print_test_result(const std::string& _name, bool succeed) {
  if (succeed)
    std::cout << _name << "\t\t\t succeed!\n";
  else
    std::cout << _name << "\t\t\t failed!\n";
}

int main(int argc, char *argv[]) {

  ///////////////////////
  // test open
  // and save
  // PNG image
  ///////////////////////
  bool rst;
  int run_time = 10;

  //read in an image for future testing
  qivon::Image<unsigned char> img = qivon::readPngFile("wallpaper.png");

  //output the information of opened image
  std::cout << "The dimension of image return is: " << img.width() << ", " << img.height() << "\n";
  std::cout << "Channels: " << img.channels() << "\n";
  std::cout << "Image is empty: " << img.isEmpty() << "\n";

  ////////////////////////////////////
  // IO Test
  ////////////////////////////////////
  //test import png image
  rst = test_read_png("wallpaper.png");
  update_test_counter(rst);
  print_test_result("test_read_png", rst);

  //test export png image
  rst = test_write_png("result.png", img);
  update_test_counter(rst);
  print_test_result("test_write_png", rst);

  //test import jpg image
  rst = test_read_jpg("wallpaper.jpg");
  update_test_counter(rst);
  print_test_result("test_read_jpg", rst);

  //test export jpg image
  rst = test_write_jpg("result.jpg", img, 85);
  update_test_counter(rst);
  print_test_result("test_write_jpg", rst);
  ////////////////////////////////////


  ////////////////////////////////////
  //Geometry Test
  ////////////////////////////////////
  //test resize image
  rst = test_resize_image(img, 400, 300);
  update_test_counter(rst);
  print_test_result("test_resize_image", rst);

  //test crop image
  rst = test_crop_image(img, 192, 108, 1536, 864);
  update_test_counter(rst);
  print_test_result("test_crop_image", rst);
  ////////////////////////////////////


  ////////////////////////////////////
  //Filter Test
  ////////////////////////////////////
  //convert image to grayscale for mean testing
  qivon::Image<unsigned char> gray;
  qivon::toGrayscale(img, gray);

  //test mean filter 3x3
  rst = test_mean_filter_3x3(gray, run_time);
  update_test_counter(rst);
  print_test_result("test_mean_filter_3x3", rst);
  ////////////////////////////////////



  ////////////////////////////////////
  // Color Test
  ////////////////////////////////////
  //test convert to grayscale
  rst = test_to_grayscale(img, run_time);
  update_test_counter(rst);
  print_test_result("test_to_grayscale", rst);

  //test convert from rgb to bgr
  rst = test_rgb_to_bgr(img, run_time);
  update_test_counter(rst);
  print_test_result("test_rgb_to_bgr", rst);

  qivon::Image<unsigned char> bgr;
  qivon::rgb_to_bgr(img, bgr);

  //test convert bgr to rgb
  rst = test_bgr_to_rgb(bgr, run_time);
  update_test_counter(rst);
  print_test_result("test_bgr_to_rgb", rst);

  //test gamma correction
  rst = test_gamma_correction(img, 1.5, run_time);
  update_test_counter(rst);
  print_test_result("test_gamma_correction", rst);

  //test brightness adjustment
  rst = test_brightness_adjustment(img, 50, run_time);
  update_test_counter(rst);
  print_test_result("test_brightness_adjustment", rst);

  //test contrast adjustment
  rst = test_contrast_adjustment(img, 110, run_time);
  update_test_counter(rst);
  print_test_result("test_contrast_adjustment", rst);

  //test white balance adjustment
  rst = test_white_balance_adjustment(img, 15000, run_time);
  update_test_counter(rst);
  print_test_result("test_white_balance_adjustment", rst);

  //test hue adjustment
  rst = test_hue_adjustment(img, 50, run_time);
  update_test_counter(rst);
  print_test_result("test_hue_adjustment", rst);

  //test saturation adjustment
  rst = test_saturation_adjustment(img, 50, run_time);
  update_test_counter(rst);
  print_test_result("test_saturation_adjustment", rst);
  ////////////////////////////////////


  //print the test result
  std::cout << "Test:\t\t\t " << g_test_count << std::endl;
  std::cout << "Test succeed:\t " << g_test_succeed << std::endl;
  std::cout << "Test failed:\t " << g_test_failed << std::endl;

  exit(0);
}
