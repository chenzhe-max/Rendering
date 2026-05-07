//有三角形这个形状之后就可以开始定义模型了
#pragma once

#include <filesystem>
#include "triangle.hpp"

class Model : public Shape
{
public:
    Model(const std::vector<Triangle> &triangles) : triangles(triangles) {}
    Model(const std::filesystem::path &filename); //加载模型文件
    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;

private:
    std::vector<Triangle> triangles;
};