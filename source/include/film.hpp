//相机的胶片，存储了每个像素的颜色，并且负责把最终的图像存储成一张图片
#pragma once
#include <filesystem>
#include <vector> //颜色都存在一个vector里
#include <glm/glm.hpp>

class Film{
public:
    Film(size_t width, size_t height);

    void save(const std::filesystem::path &filename);  //要用到c++17.所以CmakeLists才需要改一下

    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }
    glm::vec3 getPixel(size_t x, size_t y) { return pixels[ y * width + x]; }
    void setPixel(size_t x, size_t y, const glm::vec3 &color) { pixels[y * width + x] = color;}

private:
    size_t width;
    size_t height;
    std::vector<glm::vec3> pixels;
};