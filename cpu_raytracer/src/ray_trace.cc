#include "ray_trace.h"

#include <iostream>
#include <cmath>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>

auto dot_normalized(glm::vec3 v1, glm::vec3 v2) -> float
{
    return glm::dot(glm::normalize(v1), glm::normalize(v2));
}

auto debug(std::string_view str, glm::vec3 a) -> void
{
    spdlog::debug("debug: {}: {}, {}, {}\n", str, a.x, a.y, a.z);
}

auto Sphere::getIntersection([[maybe_unused]]Ray ray) const -> Intersection {
    Intersection i{
        id,
        Intersection::Type::None,
        {},
        {},
        material
    };

    using std::pow, glm::dot, glm::length;

    const auto D = ray.origin - centre;
    const auto discriminant = pow(dot(ray.dir, D), 2) - (pow(length(D), 2) - pow(radius, 2));

    if(discriminant < 0){
        return i; // no intersection
    }

    const auto a = -dot(ray.dir, D);
    const auto t = static_cast<float>(std::min(a + discriminant, a - discriminant));

    i.point = ray.origin + t * ray.dir;
    i.normal = glm::normalize(i.point - centre);
    i.type = Intersection::Type::Object;
    return i;
}

Triangles::Triangles(gsl::span<const glm::vec3> model, int id) 
    : Shape(id, {})
{
    for(auto t = std::begin(model); t < std::end(model) - 2; t += 3){
        triangles.push_back({*t, *(t+1), *(t+2)});
    }
}

auto Triangles::intersectTriangle(Ray ray, Triangle triangle) const -> Intersection {
    // From https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    constexpr auto epsilon = 0.0000001f;
    const auto vert0 = triangle.p1;
    const auto vert1 = triangle.p2;
    const auto vert2 = triangle.p3;

    const auto edge1 = vert1 - vert0;
    const auto edge2 = vert2 - vert0;

    const auto h = glm::cross(ray.dir, edge2);
    const auto a = glm::dot(edge1, h);

    if(std::abs(a) < epsilon) {
        return {}; // no intersection
    }

    const auto f = 1.0f / a;
    const auto s = ray.origin - vert0;
    const auto u = f * glm::dot(s, h);

    if(u < 0.0f || u > 1.0f) {
        return {}; // no intersection
    }

    const auto q = glm::cross(s, edge1);
    const auto v = f * glm::dot(ray.dir, q);
    if(v < 0.0f || u + v > 1.0f) {
        return {}; // no intersection
    }

    const auto t = f * glm::dot(edge2, q);

    if(t > epsilon) {
        return {
            id,                                         // id
            Intersection::Type::Object,
            ray.origin + ray.dir * t,                   // point
            glm::normalize(glm::cross(edge1, edge2)),   // normal
            material                                    // material
        };
    }
    return {}; // no intersection
}

auto Triangles::getIntersection(Ray ray) const -> Intersection
{
    auto min = std::numeric_limits<float>::max();
    auto result = Intersection{
        -1,
        Intersection::Type::None,
        {},
        {},
        material
    };

    for(auto tmp = std::begin(triangles); tmp != std::end(triangles); tmp++)
    {
        auto p = intersectTriangle(ray, *tmp);
        if(p.type == Intersection::Type::Object && glm::distance(p.point, ray.origin) < min) {
            min = glm::distance(p.point, ray.origin);
            result = p;
        }
    }
    return result;
}

auto Plane::getIntersection(Ray ray) const -> Intersection
{
    auto result = Intersection{
        id,         // id
        Intersection::Type::None,
        {},         // point
        normal,     // normal
        material    // material
    };

    if(glm::dot(normal, ray.dir) >= 0){
        return result;
    }

    const auto s = glm::dot(point - ray.origin, normal) / glm::dot(ray.dir, normal);
    result.type = Intersection::Type::Object;
    result.point = ray.origin + s * ray.dir;

    return result;
}