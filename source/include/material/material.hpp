//定义材质
#pragma once

#include <glm/glm.hpp>

struct Material {
    glm::vec3 albedo = { 1, 1, 1 };
    bool is_specular = false;  //is_specular定义材质是不是镜面反射，false就是漫反射
    glm::vec3 emissive = { 0, 0, 0 };
};