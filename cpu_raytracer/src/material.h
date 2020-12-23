#pragma once

#include <glm/vec3.hpp>

struct ObjectMaterial {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 reflectionStrength;
    float specularCoefficient;
};

constexpr auto goldMaterial() -> ObjectMaterial;
constexpr auto brassMaterial() -> ObjectMaterial;