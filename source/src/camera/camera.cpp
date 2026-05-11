#include "camera/camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(Film &film, const glm::vec3 &pos, const glm::vec3 &viewpoint, float fovy) 
    : film(film), pos(pos)
{
    //camera_from_clip其实就是透视投影矩阵的逆矩阵
    //glm::perspective()就是创建一个透视投影矩阵，需要4个参数
    //参数分别是：相机垂直视野角度（需要用弧度，因此用glm::radiant()来转成弧度）、胶片/屏幕宽高比（决定画面会不会拉伸）、最近能看到的距离（近裁剪面）、最远能看到的距离（远裁剪面）
    camera_from_clip = glm::inverse(glm::perspective(
        glm::radians(fovy),
        static_cast<float>(film.getWidth()) / static_cast<float>(film.getHeight()),
        1.f, 
        2.f
    ));
    //world_from_camera就是glm里一个叫做lookAt函数的逆矩阵
    //glm::lookAt()需要3个参数，分别是相机位置、相机所看向的一个点、头顶向上方向(一般是y轴正方向，也就是glm::vec3(0, 1, 0))
    world_from_camera = glm::inverse(glm::lookAt(pos, viewpoint, {0, 1, 0}));
}

Ray Camera::generateRay(const glm::ivec2 &pixel_coord, const glm::vec2 &offset ) const
{
    //生成光线首先是屏幕空间下的像素位置到NDC空间下的XY坐标  因为是屏幕到ndc，所以先用xy二维
    glm::vec2 ndc = (glm::vec2(pixel_coord) + offset) / glm::vec2(film.getWidth(), film.getHeight());
    //NDC的y轴是向上的、屏幕空间的y轴是向下的，所以要反转
    ndc.y = 1.f - ndc.y;
    //ndc的xy的范围要从0到1映射到-1到1  这个项目默认用左手坐标系，z的范围是0到1，所以CmakeList里要设置一下宏
    ndc = 2.f * ndc - 1.f;
    //这是因为在Camera中设定了near是1，构造了一个在near上的点，所有相机发射的光线都会穿过near plane
    glm::vec4 clip { ndc, 0, 1};
    glm::vec3 world = world_from_camera * camera_from_clip * clip;
    //光线起点就是相机的位置，方向就是屏幕上的像素点对应的世界空间的像素点减去世界空间下的相机的位置，再归一化一下就行了
    return Ray{
        pos,
        glm::normalize(world - pos)
    };
}
