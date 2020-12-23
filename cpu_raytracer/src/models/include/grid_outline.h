//
// Created by Jack Glass on 2020-11-11.
//

#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include "point.h"

namespace mdl {
  enum class Plane {
    XY,
    XZ,
    YZ
  };

  class GridOutline
  {
  private:
    std::vector<gl::Point> m_points;

  public:
    GridOutline(glm::vec2 bounds, float step, float level, Plane plane);

    // getters
    [[nodiscard]]
    auto points() const {return m_points;}
  };
}// namespace mdl
