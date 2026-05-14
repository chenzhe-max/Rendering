//有三角形这个形状之后就可以开始定义模型了
#pragma once

#include "accelerate/bvh.hpp"
#include "triangle.hpp"
#include <filesystem>

class Model : public Shape {
public:
    Model(const std::vector<Triangle> &triangles) {
        auto ts = triangles;  //build函数的参数是右值引用，所以要先将三角形列表拷贝一次，再将拷贝出来的三角形列表作为右值引用传进去
        bvh.build(std::move(ts));  
    }

    Model(const std::filesystem::path &filename);

    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;
    Bounds getBounds() const override { return bvh.getBounds(); }
private:
    BVH bvh {};  //bvh存储了三角形信息，所以BVH bvh {}就够了
};