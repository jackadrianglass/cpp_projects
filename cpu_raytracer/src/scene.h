#pragma once

#include <memory>
#include <vector>
#include <glm/vec3.hpp>

struct Shape;

struct Scene {
    glm::vec3 lightPos;
    glm::vec3 lightCol;
    float ambiantFactor;
    std::vector<std::shared_ptr<Shape>> shapesInScene;
};

auto initScene1() -> Scene;
auto initScene2() -> Scene;