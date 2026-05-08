//该文件用于实现一个三角形的形状

#pragma once

#include "shape.hpp"

struct Triangle : public Shape{
    //两个构造函数，一个是有顶点和法向量，一个是只有顶点，法向量要自己算
    Triangle(
        const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2,
        const glm::vec3 &n0, const glm::vec3 &n1, const glm::vec3 &n2
    ) : p0(p0), p1(p1), p2(p2), n0(n0), n1(n1), n2(n2) {}

    Triangle(
        const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2
    ) : p0(p0), p1(p1), p2(p2) {
        glm::vec3 e1 = p1 - p0;
        glm::vec3 e2 = p2 - p0;
        glm::vec3 normal = glm::normalize(glm::cross(e1, e2));
        n0 = normal;
        n1 = normal;
        n2 = normal;//法向量一样是因为是平坦三角形，三个点在一个面上
    }
    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;//因为是继承的shape所以用override

    glm::vec3 p0, p1, p2; //三个顶点的位置
    glm::vec3 n0, n1, n2; //三个顶点的法向量
};