#pragma once

#include "shape.hpp"

struct Sphere : public Shape
{
    Sphere (const glm::vec3 &center, float radius) : center(center), radius(radius) {}
    glm::vec3 center;
    float radius;

    //std::optional用于表示值可能存在也可能不存在的情况，c++17
    //用于相交测试的函数，与传进来的光线进行相交测试，没有就返回空，有的话就返回交点与光线原点的距离
    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;
};    
