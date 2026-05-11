//人眼对光线的感知不是线性的，所以要做Gamma矫正
//RGB是光照和材质共同作用的结果

#pragma once

#include "../material/material.hpp"
#include <glm/glm.hpp>

class RGB {
public:
    RGB(int r, int g, int b) : r(r), g(g), b(b) {}

    //这个构造函数能将物理意义上的光照强度转换为RGB，就是下面的隐式转换的函数反过来
    RGB(const glm::vec3 &color) {
        r = glm::clamp<int>(glm::pow(color.x, 1.0 / 2.2) * 255, 0, 255);
        g = glm::clamp<int>(glm::pow(color.y, 1.0 / 2.2) * 255, 0, 255);
        b = glm::clamp<int>(glm::pow(color.z, 1.0 / 2.2) * 255, 0, 255);
    }

    //用于隐式转换的函数，RBG颜色转为物理意义上的光照强度，原理就是0-255变成0-1再乘上2.2次方
    operator glm::vec3() const  {
        return glm::vec3 {
            glm::pow(r / 255.f, 2.2),
            glm::pow(g / 255.f, 2.2),
            glm::pow(b / 255.f, 2.2),
        };
    }
public:
    int r, g, b;
};