#include "renderer/path_tracing_renderer.hpp"
#include "util/frame.hpp"

glm::vec3 PathTracingRenderer::renderPixel(const glm::ivec2 &pixel_coord) {
    auto ray = camera.generateRay(pixel_coord, { rng.uniform(), rng.uniform() });
    glm::vec3 beta = { 1, 1, 1 };
    glm::vec3 L = { 0, 0, 0 };  //就是radiance
    float q = 0.9;

    while (true) {
        auto hit_info = scene.intersect(ray);
        if (hit_info.has_value()) {
            L += beta * hit_info->material->emissive; //先让光源对像素处的radiance产生一个贡献再俄罗斯轮盘赌，这样光源上就没有黑点了，图像质量也提高了一点

            if (rng.uniform() > q) //俄罗斯轮盘赌算法来判断是否要中止当前的计算
            {
                break;
            }
            beta /= q;

            Frame frame(hit_info->normal);
            glm::vec3 light_direction;
            if (hit_info->material) {
                glm::vec3 view_direction = frame.localFromWorld(-ray.direction);
                light_direction = hit_info->material->sampleBRDF(view_direction, beta, rng);
            } else {
                break;
            }

            ray.origin = hit_info->hit_point;
            ray.direction = frame.worldFromLocal(light_direction);
        } else {
            break;
        }
    }

    return L;
}