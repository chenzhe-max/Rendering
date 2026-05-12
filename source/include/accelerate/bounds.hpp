//用轴对齐包围盒来提高性能

#pragma once

#include "camera/ray.hpp"

struct Bounds {
    Bounds() : b_min(0), b_max(0) {}

    Bounds(const glm::vec3 &b_min, const glm::vec3 &b_max) : b_min(b_min), b_max(b_max) {}

    //用于拓展当前的包围盒
    void expand(const glm::vec3 &pos) {
        b_min = glm::min(pos, b_min);
        b_max = glm::max(pos, b_max);
    }

    //用于判断光线是不是和包围盒相交
    bool hasIntersection(const Ray &ray, float t_min, float t_max) const;

    glm::vec3 b_min;
    glm::vec3 b_max;
};