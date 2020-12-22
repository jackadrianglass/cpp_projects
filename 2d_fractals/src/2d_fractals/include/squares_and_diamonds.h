//
// Created by Jack Glass on 2020-09-28.
//

#ifndef MYPROJECT_SQUARES_AND_DIAMONDS_H
#define MYPROJECT_SQUARES_AND_DIAMONDS_H

#include <array>
#include <vector>
#include <glm/vec3.hpp>

#include "square.h"

namespace fractals{
  auto squares_and_diamonds(int iter) -> std::vector<gl::shapes::Square>;
  auto calc_midpoint_square(const gl::shapes::Square& square, const glm::vec3& colour) -> gl::shapes::Square;
}

#endif//MYPROJECT_SQUARES_AND_DIAMONDS_H
