#include "material.h"

constexpr auto goldMaterial() -> ObjectMaterial
{
    // https://en.wikibooks.org/wiki/Blender_3D:_Noob_to_Pro/Every_Material_Known_to_Man/Gold
    auto diffuse = glm::vec3{0.912, 0.782, 0.082};
    return {
        0.1f * diffuse,                     //< ambiant
        diffuse,                            //< diffuse
        glm::vec3{1.0, 0.913, 0.8},         //< specular
        0.1f * glm::vec3{0.857, 0.90, 0.70},//< reflectionStrength
        27.8f                               //< specularCoefficient
    };
}

constexpr auto brassMaterial() -> ObjectMaterial
{
    return {
        glm::vec3{0.33f, 0.22f, 0.03f},    //< ambiant
        glm::vec3{0.78f, 0.57f, 0.11f},    //< diffuse
        glm::vec3{0.99f, 0.91f, 0.81f},    //< specular
        glm::vec3(0.1f),                 //< reflectionStrength
        27.8f                           //< specularCoefficient
    };
}