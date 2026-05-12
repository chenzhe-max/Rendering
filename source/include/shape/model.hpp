//有三角形这个形状之后就可以开始定义模型了
#pragma once

#include "accelerate/bounds.hpp"
#include "triangle.hpp"
#include <filesystem>

class Model : public Shape {
public:
    Model(const std::vector<Triangle> &triangles) : triangles(triangles) { build(); }

    Model(const std::filesystem::path &filename);

    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;
private:
    void build();  //遍历所有三角形并且让包围盒包括三角形
private:
    Bounds bounds {};
    std::vector<Triangle> triangles;
};