#include <vector>
#include "stb_implementation.h"

using namespace std;

class Image
{
private:
    int height;
    int width;
    vector<float> data;

    int getIndex(int i, int j, int c);

    float* convertToRGB();

public:
    Image(int h, int w);

    void setData(int row, int column, int channel, float value);

    int getHeight() { return height; }
    int getWeight() { return width; }

    void writeToHdr();
};

Image::Image(int h, int w){
    height = h;
    width = w;
    data.resize(4 * h * w);
}

int Image::getIndex(int i, int j, int c){
    return 4 * (i + width * j) + c;
}

void Image::setData(int row, int column, int channel, float value){
    data[getIndex(row, column, channel)] = value;
}

float* Image::convertToRGB(){
    vector<float> result;

    for(int i = 0; i < 4 * height * width; i+=4){
        result.push_back(data[i]);
        result.push_back(data[i+1]);
        result.push_back(data[i+2]);
    }

    return &result[0];
}

void Image::writeToHdr(){
    stbi_write_hdr("img001", width, height, 3, convertToRGB());
}