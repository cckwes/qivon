#include <image.h>
#include <pngio.h>
#include <jpgio.h>
#include <color.h>
#include <filter.h>
#include <iostream>

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
  qivon::Image<unsigned char> gamma;

  qivon::gamma_correction(img, gamma, 1.5);

  rst = qivon::writePngFile("gamma.png", gamma);

  if (rst)
    std::cout << "Image exported successfully\n";
  else
    std::cout << "Problem encountered when export image\n";
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