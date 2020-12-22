//
// Created by Jack Glass on 2020-09-28.
//

#ifndef MYPROJECT_FRACTAL_MATH_H
#define MYPROJECT_FRACTAL_MATH_H

#include <glm/vec3.hpp>
namespace fractals {
  [[nodiscard]]
  auto midpoint(glm::vec3 first, glm::vec3 second) -> glm::vec3;

  [[nodiscard]]
  auto midpoint(glm::vec3 first, glm::vec3 second, float frac_distance_from_first) -> glm::vec3;

  [[nodiscard]]
  auto rotate_2d(glm::vec3 vec, float deg) -> glm::vec3;

  [[nodiscard]]
  auto angle_from_x_axis(glm::vec3 vec) -> float;

  [[nodiscard]]
  auto deg_to_rad(float deg) -> float;
}
#endif//MYPROJECT_FRACTAL_MATH_H
