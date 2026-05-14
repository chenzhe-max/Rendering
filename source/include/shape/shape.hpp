//该文件用于抽象出一个形状

#pragma once

#include "../camera/ray.hpp"
#include "../accelerate/bounds.hpp"
#include <optional>

struct Shape
{
    //返回距离光线原点的距离、交点的位置、交点处的法向量，所以用ray.hpp中的HitInfo
    virtual std::optional<HitInfo> intersect(
        const Ray &ray, 
        float t_min,  //t_min和t_max用于指定光线的交点到光线原点的距离在什么范围，超过这个范围就视为没有相交
        float t_max  //函数形参，默认参数只能向右扎堆,可以t_max有默认参数t_min没有，但不能反过来，因为t_max在右边
    ) const = 0;
    virtual Bounds getBounds() const { return {}; }  //默认返回退化的包围盒，因为比如plane是无限大的包围盒，需要单独处理，就直接返回一个退化的包围盒
};
