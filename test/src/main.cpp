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
  update_test_counter(test_read_png("wallpaper.png"));

  //test export png image
  update_test_counter(test_write_png("result.png", img));

  //test import jpg image
  update_test_counter(test_read_jpg("wallpaper.jpg"));

  //test export jpg image
  update_test_counter(test_write_jpg("result.jpg", img, 85));
  ////////////////////////////////////


  ////////////////////////////////////
  //Geometry Test
  ////////////////////////////////////
  //test resize image
  update_test_counter(test_resize_image(img, 400, 300));

  //test crop image
  update_test_counter(test_crop_image(img, 192, 108, 1536, 864));
  ////////////////////////////////////


  ////////////////////////////////////
  //Filter Test
  ////////////////////////////////////
  //test mean filter 3x3
  update_test_counter(test_mean_filter_3x3(img, run_time));
  ////////////////////////////////////



  ////////////////////////////////////
  // Color Test
  ////////////////////////////////////
  //test convert to grayscale
  update_test_counter(test_to_grayscale(img, run_time));

  //test convert from rgb to bgr
  update_test_counter(test_rgb_to_bgr(img, run_time));

  qivon::Image<unsigned char> bgr;
  qivon::rgb_to_bgr(img, bgr);

  //test convert bgr to rgb
  update_test_counter(test_bgr_to_rgb(bgr, run_time));

  //test gamma correction
  update_test_counter(test_gamma_correction(img, 1.5, run_time));

  //test brightness adjustment
  update_test_counter(test_brightness_adjustment(img, 50, run_time));

  //test contrast adjustment
  update_test_counter(test_contrast_adjustment(img, 110, run_time));

  //test white balance adjustment
  update_test_counter(test_white_balance_adjustment(img, 15000, run_time));

  //test hue adjustment
  update_test_counter(test_hue_adjustment(img, 50, run_time));

  //test saturation adjustment
  update_test_counter(test_saturation_adjustment(img, 50, run_time));
  ////////////////////////////////////


  //print the test result
  std::cout << "Test:\t\t " << g_test_count << std::endl;
  std::cout << "Test succeed:\t " << g_test_succeed << std::endl;
  std::cout << "Test failed:\t " << g_test_failed << std::endl;

  exit(0);
}
