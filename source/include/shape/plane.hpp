//定义一个平面，平面上的一个点以及平面的法向量来定义一个平面，也是shape，也要一个相交测试的函数

#pragma once

#include "shape.hpp"

struct Plane : public Shape {
    Plane(const glm::vec3 &point, const glm::vec3 &normal) : point(point), normal(glm::normalize(normal)) {}

    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;

    glm::vec3 point;
    glm::vec3 normal;
};