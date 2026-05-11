//定义了漫反射过程中的局部反射空间
//因为这个空间只涉及了方向的计算，所以不用存储坐标系的原点，只存储坐标轴的方向就可以了

//构造函数传入的法线是定义在世界空间的，根据法线求出三个坐标轴的方向
//三个坐标轴代表了局部反射空间的坐标轴在世界空间中的方向
//局部反射空间的三个轴：y轴是法线的方向；x轴通过叉乘得到，就是0，1，0叉乘法线， 0，1，0就是世界坐标系的y轴的方向，再用x轴和y轴叉乘得到z轴方向
//如果法线就是0，1，0的话，那就叉乘0，0，1即可
//世界转局部：世界空间的坐标点乘局部反射坐标的xyz三个轴就能得到局部反射空间下的方向
//局部转世界：局部反射空间的xyz分量分别乘上局部反射坐标的xyz轴再将结果相加

#pragma once

#include <glm/glm.hpp>

class Frame {
public:
    Frame(const glm::vec3 &normal);

    glm::vec3 localFromWorld(const glm::vec3 &direction_world) const;
    glm::vec3 worldFromLocal(const glm::vec3 &direction_local) const;
private:
    glm::vec3 x_axis, y_axis, z_axis;
};