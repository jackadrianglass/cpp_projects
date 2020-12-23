//
// Created by Jack Glass on 2020-11-14.
//

#pragma once

#include "grid_outline.h"
#include "curve.h"

namespace mdl {
  class Grid
  {
  private:
    std::vector<gl::Point> m_points;
    size_t m_rowWidth;
    Algorithm m_type;

    mutable std::vector<gl::Point> m_vertices{};
    mutable std::vector<unsigned int> m_indices{};

  public:
    Grid(glm::vec2 bounds, float step, mdl::Plane plane, Algorithm type, unsigned int seed);

    // getters
    [[nodiscard]]
    auto points() const {return m_points;}
    [[nodiscard]]
    auto vertices() const -> std::vector<gl::Point>;
    [[nodiscard]]
    auto indices() const -> std::vector<unsigned int>;

  private:
    auto initBezier() -> size_t;
    auto initBspline() -> size_t;
  };
}
