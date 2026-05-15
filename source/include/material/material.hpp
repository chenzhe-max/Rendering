//所有材质的基类
#pragma once

#include "util/rng.hpp"
#include <glm/glm.hpp>

class Material {
public:
    //用于完成重要性采样，根据传进来的观察方向，来采样每一个材质自身的BRDF函数（根据BRDF选择形状相似的pdf进行采样），并返回一个光线方向，两个方向都是定义在局部坐标系的
    virtual glm::vec3 sampleBRDF(const glm::vec3 &view_direction, glm::vec3 &beta, const RNG &rng) const = 0;
    
    //设置自发光的函数
    void setEmissive(const glm::vec3 &emissive) { this->emissive = emissive; }
public:
    glm::vec3 emissive;
};