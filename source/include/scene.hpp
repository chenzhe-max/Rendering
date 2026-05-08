//定义一个场景，场景也可以看作一种shape，场景的形状就是场景包含的所有物体的形状的总和，用vector存放

#pragma once

#include "shape.hpp"
//ShapeInstance代表一个shape的实例
struct ShapeInstance {
    const Shape *shape;
    glm::mat4 world_from_object;
    glm::mat4 object_from_world;
};

struct Scene : public Shape {
public:
    void addShape(
        const Shape *shape,
        const glm::vec3 &pos = { 0, 0, 0 },
        const glm::vec3 &scale = { 1, 1, 1 },
        const glm::vec3 &rotate = { 0, 0, 0 }
    );

    std::optional<HitInfo> intersect(
        const Ray &ray,
        float t_min = 1e-5,
        float t_max = std::numeric_limits<float>::infinity()
    ) const override;
private:
    std::vector<ShapeInstance> instances;
};