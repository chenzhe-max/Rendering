#include "shape/model.hpp"
#include "util/profile.hpp"
#include <rapidobj/rapidobj.hpp>

//要加载obj格式的文件，注意obj的定义中索引是从1开始而不是0
//比如 v 22 12 12    vn 22 12 12     f 4//3 1//2 2//1  T{ 4, 1, 2  3, 2, 1}
// 4是位置的索引，3是法线的索引，4//3就是一个顶点,4 1 2是三个顶点的索引 f定义了一个三角形  这三个分别表示位置和法线和索引
//这里用rapidobj这个库来加载 ，就不用之前写的代码了 
Model::Model(const std::filesystem::path &filename)  
{
    PROFILE("Load model " + filename.string())

    auto result = rapidobj::ParseFile(filename, rapidobj::MaterialLibrary::Ignore());
    std::vector<Triangle> triangles;

    for (const auto &shape : result.shapes) {
        size_t index_offset = 0;
        for (size_t num_face_vectex : shape.mesh.num_face_vertices) {
            if (num_face_vectex == 3) 
            {
                auto index = shape.mesh.indices[index_offset];
                glm::vec3 pos0 
                {
                    result.attributes.positions[index.position_index * 3 + 0],
                    result.attributes.positions[index.position_index * 3 + 1],
                    result.attributes.positions[index.position_index * 3 + 2]
                };
                index = shape.mesh.indices[index_offset + 1];
                glm::vec3 pos1 
                {
                    result.attributes.positions[index.position_index * 3 + 0],
                    result.attributes.positions[index.position_index * 3 + 1],
                    result.attributes.positions[index.position_index * 3 + 2]
                };
                index = shape.mesh.indices[index_offset + 2];
                glm::vec3 pos2 
                {
                    result.attributes.positions[index.position_index * 3 + 0],
                    result.attributes.positions[index.position_index * 3 + 1],
                    result.attributes.positions[index.position_index * 3 + 2]
                };
                if (index.normal_index >= 0) {
                    index = shape.mesh.indices[index_offset];
                    glm::vec3 normal0 {
                        result.attributes.normals[index.normal_index * 3 + 0],
                        result.attributes.normals[index.normal_index * 3 + 1],
                        result.attributes.normals[index.normal_index * 3 + 2]
                    };
                    index = shape.mesh.indices[index_offset + 1];
                    glm::vec3 normal1 {
                        result.attributes.normals[index.normal_index * 3 + 0],
                        result.attributes.normals[index.normal_index * 3 + 1],
                        result.attributes.normals[index.normal_index * 3 + 2]
                    };
                    index = shape.mesh.indices[index_offset + 2];
                    glm::vec3 normal2 {
                        result.attributes.normals[index.normal_index * 3 + 0],
                        result.attributes.normals[index.normal_index * 3 + 1],
                        result.attributes.normals[index.normal_index * 3 + 2]
                    };
                    triangles.push_back(Triangle {
                        pos0, pos1, pos2, normal0, normal1, normal2
                    });
                } else {
                    triangles.push_back(Triangle {
                        pos0, pos1, pos2
                    });
                }
            }
            index_offset += num_face_vectex;
        }

    }
    bvh.build(std::move(triangles));
  
}

//这里的相交测试就是遍历所有的三角形，寻找最近的交点
std::optional<HitInfo> Model::intersect(const Ray &ray, float t_min, float t_max) const 
{
    return bvh.intersect(ray, t_min, t_max);
}
