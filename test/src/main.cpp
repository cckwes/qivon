#include <image.h>
#include <pngio.h>
#include <jpgio.h>
#include <color.h>
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
  qivon::Image<unsigned char> grayscale_img = qivon::toGrayscale(img);

  rst = qivon::writePngFile("grayscale.png", grayscale_img);

  if (rst)
    std::cout << "Grayscale image exported successfully\n";
  else
    std::cout << "Problem encountered when export grayscale image\n";
  ///////////////////////


  ///////////////////////
  // test open JPEG
  ///////////////////////
  img = qivon::readJpgFile("wallpaper.jpg");

  std::cout << "The dimension of image return is: " << img.width() << ", " << img.height() << "\n";
  std::cout << "Channels: " << img.channels() << "\n";
  std::cout << "Image is empty: " << img.isEmpty() << "\n";

  rst = qivon::writePngFile("from_jpg.png", img);

  if (rst)
    std::cout << "Image exported successfully\n";
  else
    std::cout << "Problem encountered when export image\n";
  ///////////////////////

  exit(0);
}