//
// Created by Jack Glass on 2020-09-29.
//

#ifndef MYPROJECT_KOCH_SNOWFLAKE_H
#define MYPROJECT_KOCH_SNOWFLAKE_H

#include <vector>
#include <glm/vec3.hpp>
#include "triangle.h"

namespace fractals{

  [[nodiscard]]
  auto koch_snowflake(gl::shapes::Triangle triangle, int iter) -> std::vector<glm::vec3>;
  auto koch_snowflake_algo(std::vector<glm::vec3>& points, glm::vec3 A, glm::vec3 B, int iter) -> void;
}


#endif//MYPROJECT_KOCH_SNOWFLAKE_H
