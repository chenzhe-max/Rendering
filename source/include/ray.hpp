//用于定义光线
#pragma once 

#include <glm/glm.hpp>

class Ray
{
public:
    glm::vec3 origin;
    glm::vec3 direction;

    //求光线交点的函数，t传播的距离，返回光线在原点向direction方向传播t距离后的点
    glm::vec3 hit(float t) const {return origin + t * direction; }
    //将光线从世界坐标转换成对象坐标
    Ray objectFromWorld(const glm::mat4 &object_from_world) const;
};

struct HitInfo{
    float t;
    glm::vec3 hit_point;
    glm::vec3 normal;
};