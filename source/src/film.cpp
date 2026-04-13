#include "film.hpp"
#include <fstream>
Film::Film(size_t width, size_t height) : width(width), height(height)
{
    pixels.resize(width * height);
}

void Film::save(const std::filesystem::path &filename)
{
    //要用到PPM的文件格式
    /*文件第一行是P3 指定图片是三通道的RGB组成，但是用P3的话就是ascii存储颜色，保存文件太慢了
    所以用P6，二进制，这样够快，颜色的每个分量是分到8bit，也就是1字节*/
    //第二行存放宽高，比如1920 1080
    //第三行存储颜色的每个分量的最大值，比如255
    //之后每一行写颜色的数值，比如255 0 0 
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << width << ' ' << height << "\n255\n";

    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
        {
            const glm::vec3 &color = getPixel(x, y);
            //glm::vec3 就是 float vec3   glm::ivec3就是int vec3 glm::u8vec3 就是 uint8_t vec3
            //uint8_t就是8位无符号，范围是0-255
            glm::u8vec3 color_i = glm::clamp(color * 255.f, 0.f, 255.f);
            file << color_i.x << color_i.y << color_i.z;//这是P6的写法
            //file << color_i.x << ' ' << color_i.y << ' ' << color_i.z << '\n';这是P3的写法

        }
    }
}