#include "camera/film.hpp"
#include "thread/thread_pool.hpp"
#include "util/rgb.hpp"
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
    //file << color_i.x << ' ' << color_i.y << ' ' << color_i.z << '\n';这是P3的写法
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << width << ' ' << height << "\n255\n";

   std::vector<uint8_t> buffer(width * height * 3);

    thread_pool.parallelFor(width, height, [&](size_t x, size_t y) {
        auto pixel = getPixel(x, y);
        RGB rgb(pixel.color / static_cast<float>(pixel.sample_count));
        auto idx = (y * width + x) * 3;
        buffer[idx + 0] = rgb.r;
        buffer[idx + 1] = rgb.g;
        buffer[idx + 2] = rgb.b;
    }, false);
    thread_pool.wait();

    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
}