#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <gsl/span>

#include "material.h"

auto dot_normalized(glm::vec3 v1, glm::vec3 v2) -> float;
auto debug(std::string_view str, glm::vec3 a) -> void;


struct Ray {
    glm::vec3 origin;
    glm::vec3 dir;
};

struct Intersection {
    enum class Type {
        None,
        Object,
        LightSource
    };

    int id{-1};
    Type type;
    glm::vec3 point;
    glm::vec3 normal;

    ObjectMaterial material;
};

struct Shape {
    int id;
    ObjectMaterial material;

    explicit Shape(int i, ObjectMaterial m)
    : id{i}
    , material{m}
    {}

    Shape() = default;
    virtual auto getIntersection(Ray ray) const -> Intersection = 0;
};

struct Triangle {
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
};

struct Triangles : public Shape {
    explicit Triangles(gsl::span<const glm::vec3> triangles, int id);
    virtual ~Triangles() = default;
    std::vector<Triangle> triangles;

    auto getIntersection(Ray ray) const -> Intersection override;
    auto intersectTriangle(Ray ray, Triangle t) const -> Intersection;
};

struct Sphere : public Shape {
    glm::vec3 centre;
    float radius;

    explicit Sphere(glm::vec3 c, float r, int i)
        : Shape(i, {})
        , centre{c}
        , radius{r}
    {}

    Sphere() = default;
    auto getIntersection(Ray ray) const -> Intersection override;
};

struct Plane : public Shape {
    glm::vec3 point;
    glm::vec3 normal;

    explicit Plane(glm::vec3 p, glm::vec3 n, int id)
    : Shape(id, {})
    , point{p}
    , normal{glm::normalize(n)}
    {}

    Plane() = default;
    auto getIntersection(Ray ray) const -> Intersection override;
};