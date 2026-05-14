#pragma once

#include "bounds.hpp"
#include "../shape/triangle.hpp"

struct BVHTreeNode {
    Bounds bounds {}; //BVH是树形结构，所以要定义一个节点出来，每个节点存储一个自己的包围盒
    std::vector<Triangle> triangles; //代表节点中所有三角形
    BVHTreeNode *children[2];  //二叉树结构
    size_t depth;
    size_t split_axis;
    //遍历三角形并更新包围盒
    void updateBounds() {
        bounds = {};
        for (const auto &triangle : triangles) {
            bounds.expand(triangle.getBounds());
        }
    }
};

//线形节点
struct alignas(32) BVHNode {  //BVHNode正好是32字节（bounds是6个float），来个32字节对齐进一步提高缓存命中率
    Bounds bounds {};
    union {
        int child1_index;  //存索引不存三角形也是为了缓存命中率
        int triangle_index; //叶子节点只用triangle_index，非叶子节点只用child1_index，所以用union，节省下空间
    };
    uint16_t triangle_count;
    uint8_t split_axis;  /*划分包围盒的划分轴，右边来的光线当然要先检测右边的节点的包围盒，
    要看光线方向在这个节点的划分轴上的分量的正负来决定遍历的顺序，分量是负就让第一个子节点放入到栈中，下一个遍历的节点等于这个节点的第二个子节点的索引*/
};

//加速结构构建的一些信息
struct BVHState {
    size_t total_node_count {};  //总节点数
    size_t leaf_node_count {};   //叶子节点数
    size_t max_leaf_node_triangle_count {};  //叶子节点中三角形数量的最大值
    size_t max_leaf_node_depth {};
    void addLeafNode(BVHTreeNode *node) {
        leaf_node_count ++;
        max_leaf_node_triangle_count = glm::max(max_leaf_node_triangle_count, node->triangles.size());
        max_leaf_node_depth = glm::max(max_leaf_node_depth, node->depth);
    }
};

class BVHTreeNodeAllocator {
public:
    //使用内存分配器分配节点，一个个new太慢了
    BVHTreeNodeAllocator() : ptr(4096) {}

    BVHTreeNode *allocate() {
        if (ptr == 4096) {
            nodes_list.push_back(new BVHTreeNode[4096]);
            ptr = 0;
        }
        return &(nodes_list.back()[ptr++]);
    }

    ~BVHTreeNodeAllocator() {
        for (auto *nodes : nodes_list) {
            delete[] nodes;
        }
        nodes_list.clear();
    }
private:
    size_t ptr;
    std::vector<BVHTreeNode *> nodes_list;
};

//存储BVH树的根节点
class BVH : public Shape {
public:
    void build(std::vector<Triangle> &&triangles);  //构建BVH树
    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override; //相交检测
    Bounds getBounds() const override { return nodes[0].bounds; }
private:
    void recursiveSplit(BVHTreeNode *node, BVHState &state); //递归地分割BVH节点
    size_t recursiveFlatten(BVHTreeNode *node);  //将树形BVH转成线形，返回的是该节点被放入nodes数组时的索引
private:
    BVHTreeNodeAllocator allocator {};  //用内存分配器替换掉new
    BVHTreeNode *root;
    std::vector<BVHNode> nodes;  //存储转换后的线性的数据结构
    std::vector<Triangle> ordered_triangles;
};