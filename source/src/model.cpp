#include "model.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
//这里的相交测试就是遍历所有的三角形，寻找最近的交点
std::optional<HitInfo> Model::intersect(const Ray &ray, float t_min, float t_max) const {
    std::optional<HitInfo> closest_hit_info {};

    for (const auto &triangle : triangles) {
        auto hit_info = triangle.intersect(ray, t_min, t_max);
        if (hit_info.has_value()) {
            t_max = hit_info->t;
            closest_hit_info = hit_info;
        }
    }

    return closest_hit_info;
}

//要加载obj格式的文件，注意obj的定义中索引是从1开始而不是0
//比如 v 22 12 12    vn 22 12 12     f 4//3 1//2 2//1  T{ 4, 1, 2  3, 2, 1}
// 4是位置的索引，3是法线的索引，4//3就是一个顶点,4 1 2是三个顶点的索引 f定义了一个三角形  这三个分别表示位置和法线和索引  
Model::Model(const std::filesystem::path &filename)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::ifstream file(filename);
    if(!file.good()) //file.good()必须完全正常，没到末尾、没报错 才为真.file.is_open()只判断：文件是否成功打开，不管有没有读到末尾。
    {
        std::cout << "打开失败" << std::endl;
        return;
    }

    std::string line;
    char trash;
    while(!file.eof())
    {
        std::getline(file, line);
        std::istringstream iss(line);
        if (line.compare(0, 2, "v ") == 0) 
        {
        glm::vec3 position;
        iss >> trash >> position.x >> position.y >> position.z;
        positions.push_back(position);
        }
        else if (line.compare(0, 3, "vn ") == 0) 
        {
        glm::vec3 normal;
        iss >> trash >> trash >> normal.x >> normal.y >> normal.z;
        normals.push_back(normal);
        }   
        else if (line.compare(0, 2, "f ") == 0) 
        {
        // f 0//3 1//2 2//1
        // T { 0, 1, 2  3, 2, 1}
        glm::ivec3 idx_v, idx_vn;
        iss >> trash;
        iss >> idx_v.x >> trash >> trash >> idx_vn.x;
        iss >> idx_v.y >> trash >> trash >> idx_vn.y;
        iss >> idx_v.z >> trash >> trash >> idx_vn.z;
        
        triangles.push_back(Triangle(
            positions[idx_v.x - 1], positions[idx_v.y - 1], positions[idx_v.z - 1],
            normals[idx_vn.x - 1], normals[idx_vn.y - 1], normals[idx_vn.z - 1]
        ));
        }

    }
  
}
