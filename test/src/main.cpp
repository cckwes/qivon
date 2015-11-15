#include <image.h>
#include <pngio.h>
#include <iostream>

int main(int argc, char* argv[])
{
    qivon::Image<unsigned char> img = qivon::readPngFile("rocks.png");
    std::cout << "The dimension of image return is: " << img.width() << ", " << img.height() << "\n";

    exit(0);
}