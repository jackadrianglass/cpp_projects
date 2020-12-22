//
// Created by Jack Glass on 2020-09-29.
//

#ifndef MYPROJECT_DRAGON_CURVE_H
#define MYPROJECT_DRAGON_CURVE_H


#include <vector>
#include <glm/vec3.hpp>
namespace fractals {

  enum class Dir{
    Pos,
    Neg
  };
  [[nodiscard]]
  auto dragon_curve(glm::vec3 A, glm::vec3 B, int iter) -> std::vector<glm::vec3>;
  auto dragon_curve_algo(std::vector<glm::vec3>& points, glm::vec3 A, glm::vec3 B, int iter, Dir direction) -> void;
}

#endif//MYPROJECT_DRAGON_CURVE_H
