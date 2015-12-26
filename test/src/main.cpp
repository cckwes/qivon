#include <iostream>
#include <chrono>

#include <image.h>
#include <pngio.h>
#include <jpgio.h>
#include <color.h>
#include <filter.h>
#include <geometry.h>

int main(int argc, char *argv[]) {

  ///////////////////////
  //test open
  // and save
  // PNG image
  ///////////////////////
  qivon::Image<unsigned char> img = qivon::readPngFile("wallpaper.png");

  //output the information of opened image
  std::cout << "The dimension of image return is: " << img.width() << ", " << img.height() << "\n";
  std::cout << "Channels: " << img.channels() << "\n";
  std::cout << "Image is empty: " << img.isEmpty() << "\n";

  //export image
  bool rst = qivon::writePngFile("result.png", img);

  if (rst)
    std::cout << "Image exported successfully\n";
  else
    std::cout << "Problem encountered when export image\n";

  qivon::Image<unsigned char> img_deep_copy = img.deepCopy();
  qivon::writePngFile("deep_copy.png", img_deep_copy);

  qivon::Image<unsigned char> resized_img;
  qivon::resizeImage(img, resized_img, 400, 300, true);
  qivon::writePngFile("resized.png", resized_img);
  ///////////////////////


  ///////////////////////
  // test convert
  // to grayscale
  ///////////////////////
  qivon::Image<unsigned char> grayscale_img;
  qivon::toGrayscale(img, grayscale_img);

  rst = qivon::writePngFile("grayscale.png", grayscale_img);

  if (rst)
    std::cout << "Grayscale image exported successfully\n";
  else
    std::cout << "Problem encountered when export grayscale image\n";
  ///////////////////////


  ///////////////////////
  // mean filter image
  ///////////////////////
  qivon::Image<unsigned char> filtered_img;
  qivon::meanFilter3x3(grayscale_img, filtered_img);

  if (!filtered_img.isEmpty()) {
    rst = qivon::writeJpgFile("mean.jpg", filtered_img);

    if (rst)
      std::cout << "Mean image exported successfully\n";
    else
      std::cout << "Problem encountered when export mean image\n";
  }
  ///////////////////////


  ///////////////////////
  // test open JPEG
  // and export JPEG
  ///////////////////////
  img = qivon::readJpgFile("wallpaper.jpg");

  std::cout << "The dimension of image return is: " << img.width() << ", " << img.height() << "\n";
  std::cout << "Channels: " << img.channels() << "\n";
  std::cout << "Image is empty: " << img.isEmpty() << "\n";

  rst = qivon::writeJpgFile("from_jpg.jpg", img, 80);

  if (rst)
    std::cout << "Image exported successfully\n";
  else
    std::cout << "Problem encountered when export image\n";
  ///////////////////////


  ///////////////////////
  // test gamma correction
  ///////////////////////
  const size_t run_time = 10;

  qivon::Image<unsigned char> gamma;

  auto start = std::chrono::system_clock::now();

  for (size_t i = 0; i < run_time; ++i) {
    qivon::gamma_correction(img, gamma, 1.5);
  }

  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::system_clock::now() - start);

  std::cout << "Naive gamma corection uses (avg) " << elapsed.count() / run_time << "\n";

  start = std::chrono::system_clock::now();

  for (size_t i = 0; i < run_time; ++i) {
    qivon::gamma_correction_LUT(img, gamma, 1.5);
  }

  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::system_clock::now() - start);

  std::cout << "LUT gamma corection uses (avg) " << elapsed.count() / run_time << "\n";

  rst = qivon::writePngFile("gamma.png", gamma);

  if (rst)
    std::cout << "Image exported successfully\n";
  else
    std::cout << "Problem encountered when export image\n";
  ///////////////////////

  ///////////////////////
  // test brightness
  // adjustment
  ///////////////////////
  img = qivon::readJpgFile("wallpaper.jpg");

  qivon::Image<unsigned char> brightness;
  qivon::brightness_adjustment(img, brightness, 50);

  rst = qivon::writeJpgFile("brightness.jpg", brightness);
  ///////////////////////


  ///////////////////////
  // test contrast
  // adjustment
  ///////////////////////
  qivon::Image<unsigned char> contrast;
  qivon::contrast_adjustment(img, contrast, 110);

  rst = qivon::writeJpgFile("contrast.jpg", contrast);
  ///////////////////////

  
  ///////////////////////
  // test convert RGB
  // to BGR
  ///////////////////////
  qivon::Image<unsigned char> bgr_img;
  qivon::rgb_to_bgr(img, bgr_img);

  rst = qivon::writePngFile("bgr.png", bgr_img);

  if (rst)
    std::cout << "Image exported successfully\n";
  else
    std::cout << "Problem encountered when export image\n";
  ///////////////////////

  exit(0);
}