#include <iostream>
#include "image.h"

int main() {

    // Image

    const int image_width = 256;
    const int image_height = 256;

    Image img(image_height, image_width);

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            auto r = float(i) / (image_width-1);
            auto g = float(j) / (image_height-1);
            auto b = 0.25;

            

            img.setData(i, j, 0, r);
            img.setData(i, j, 1, g);
            img.setData(i, j, 2, b);
            img.setData(i, j, 3, 1.0);
        }
    }

    img.writeToHdr();
    return 0;
}