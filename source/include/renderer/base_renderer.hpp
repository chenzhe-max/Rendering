//用于存放所有的渲染器
//首先声明场景，向场景中添加形状，再把场景和相机传到一个渲染器中并且由渲染器决定如何渲染这个场景并完成渲染
#pragma once

#include "../camera/camera.hpp"
#include "../shape/scene.hpp"

//宏的参数就是具体渲染器的名字
#define DEFINE_RENDERER(Name) \
    class Name##Renderer : public BaseRenderer { \
    public: \
        Name##Renderer(Camera &camera, const Scene &scene) : BaseRenderer(camera, scene) {} \
    private: \
        glm::vec3 renderPixel(const glm::ivec3 &pixel_coord) override; \
        }; \


//所有的渲染器都要继承自这个类
class BaseRenderer {
public:
    //由其他具体的渲染器实现，返回值是物理意义上的光照
    BaseRenderer(Camera &camera, const Scene &scene) : camera(camera), scene(scene) {}
    
    //用于渲染出一张图片，参数是每个像素的样本数以及存储的图片名
    void render(size_t spp, const std::filesystem::path &filename);
private:
    virtual glm::vec3 renderPixel(const glm::ivec3 &pixel_coord) = 0;
    protected:
    Camera &camera;
    const Scene &scene;
};

