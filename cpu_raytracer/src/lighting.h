#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scene.h"
#include "material.h"
#include "ray_trace.h"

struct PhongReflection {
    Intersection intersection;
    ObjectMaterial material;
    Ray ray;
    Scene scene;

    [[nodiscard]]
    auto l() const {return glm::normalize(scene.lightPos - p()); }
    [[nodiscard]]
    auto n() const { return glm::normalize(intersection.normal); }
    [[nodiscard]]
    auto p() const -> glm::vec3 { return intersection.point; }
    [[nodiscard]]
    auto v() const { return glm::normalize(ray.origin - p()); }
    [[nodiscard]]
    auto r() const { return -glm::reflect(l(), n());}

    [[nodiscard]]
    auto La() const { return scene.ambiantFactor * scene.lightCol; }
    [[nodiscard]]
    auto Ld() const { return scene.lightCol; }
    [[nodiscard]]
    auto Ls() const { return scene.lightCol; }

    [[nodiscard]]
    auto Ka() const { return material.ambient; }
    [[nodiscard]]
    auto Kd() const { return material.diffuse; }
    [[nodiscard]]
    auto Ks() const { return material.specular; }

    [[nodiscard]]
    auto alpha() const { return material.specularCoefficient; }

    [[nodiscard]]
    auto Ia() const {
        return Ka() * La();
    }

    [[nodiscard]]
    auto Id() const {
        auto tmp = glm::dot(l(), n());
        tmp = std::max(0.0f, tmp);
        return Kd() * tmp * Ld();
    }

    [[nodiscard]]
    auto Is() const {
        auto tmp = glm::dot(r(), v());
        tmp = std::max(0.0f, tmp);
        tmp = std::pow(tmp, alpha());

        return Ks() * Ls() * tmp;
    }

    [[nodiscard]]
    auto I() const {
        return Id() + Is() + Ia();
    }
};
