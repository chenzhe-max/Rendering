#pragma once

#include "film.hpp"
#include "ray.hpp"

class Camera
{
public:
    //构造相机类要有胶片、相机位置、相机所看向的一个点、相机在垂直方向的视野角度
    Camera(Film &film, const glm::vec3 &pos, const glm::vec3 &viewpoint, float fovy);

    //我们需要一个生成光线的函数，这个函数需要屏幕空间下的像素坐标以及像素内的偏移，返回世界空间下的一根光线
    Ray generateRay(const glm::ivec2 &pixel_coord, const glm::vec2 &offset = { 0.5, 0.5}) const;

    Film &getFilm() { return film; }
    const Film &getFilm() const { return film; } //第二个const重载了getFilm函数
private:
    Film &film;
    glm::vec3 pos;
    //glm::mat4是GLM数学库中的4×4浮点数矩阵类型
    //camera_from_clip其实就是透视投影矩阵的逆矩阵
    glm::mat4 camera_from_clip; //从裁剪空间到相机空间  裁剪空间就是相机空间齐次坐标经过透视矩阵变换后的空间
    //world_from_camera就是glm里一个叫做lookAt函数的逆矩阵
    glm::mat4 world_from_camera; //从相机空间到世界空间 相机空间就是相机原点为坐标系原点，坐标系方向取决于相机观察方向；世界空间就是全局的坐标系
};

