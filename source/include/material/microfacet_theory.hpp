//微表面理论类
#pragma once

#include <glm/glm.hpp>
#include "util/rng.hpp"

class MicrofacetTheory {
    // Smith Models 史密斯模型
    // GGX Distribution GGX推导而来
    // Stretching Invariance 满足伸缩不变性
public:
    MicrofacetTheory(float alpha_x, float alpha_z);
    float normalDistribution(const glm::vec3 &microfacet_normal) const;
    float masking(const glm::vec3 &view_direction, const glm::vec3 &microfacet_normal) const;
    float heightCorrelatedMaskingShadowing(const glm::vec3 &light_direction, const glm::vec3 &view_direction, const glm::vec3 &microfacet_normal) const;

    float visibleNormalDistribution(const glm::vec3 &view_direction, const glm::vec3 &microfacet_normal) const;
    glm::vec3 sampleVisibleNormal(const glm::vec3 &view_direction, const RNG &rng) const;

    bool isDeltaDistibution() const;
private:
    float slopeDistribution(const glm::vec2 &slope) const;
    float Lambda(const glm::vec3 &direction_upper) const;
private:
    float alpha_x {}, alpha_z {};
};