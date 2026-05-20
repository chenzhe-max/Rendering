//所有材质的基类
#pragma once

#include "util/rng.hpp"
#include <glm/glm.hpp>
#include <optional>

struct BSDFSample {
    glm::vec3 bsdf;
    float pdf;
    glm::vec3 light_direction;
};

class Material {
public:
    //用于完成重要性采样，根据传进来的观察方向，来采样每一个材质自身的BRDF函数（根据BRDF选择形状相似的pdf进行采样），并返回一个光线方向，两个方向都是定义在局部坐标系的
    virtual std::optional<BSDFSample> sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, const RNG &rng) const = 0;
    // SBDF = BRDF + BTDF
    //设置自发光的函数
    void setEmissive(const glm::vec3 &emissive) { this->emissive = emissive; }
public:
    glm::vec3 emissive;
};

