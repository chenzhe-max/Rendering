//相机的胶片，存储了每个像素的颜色，并且负责把最终的图像存储成一张图片

#pragma once

#include <filesystem>
#include <vector>
#include <glm/glm.hpp>

struct Pixel {
    glm::vec3 color { 0, 0, 0 };
    int sample_count { 0 };
};

class Film {
public:
    Film(size_t width, size_t height);

    void save(const std::filesystem::path &filename);//要用到c++17.所以CmakeLists才需要改一下


    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }
    Pixel getPixel(size_t x, size_t y) { return pixels[y * width + x]; }
    void addSample(size_t x, size_t y, const glm::vec3 &color) {
        pixels[y * width + x].color += color;
        pixels[y * width + x].sample_count ++;
    }
    void clear() { pixels.clear(); pixels.resize(width * height); }  //用于清空所有像素的函数

private:
    size_t width, height;
    std::vector<Pixel> pixels;
};