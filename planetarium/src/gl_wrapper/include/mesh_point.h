//
// Created by Jack Glass on 2020-11-26.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace gl {
  struct MeshPoint
  {
    glm::vec3 vertex;
    glm::vec2 texture;
    glm::vec3 normal;
  };
}

