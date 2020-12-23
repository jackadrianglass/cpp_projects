//
// Created by Jack Glass on 2020-11-14.
//

#pragma once
#include <vector>
#include <gsl/span>
#include "point.h"

namespace mdl{
  enum class CurveType {
    open,
    closed,
  };

  auto calcRevolvedSurface(gsl::span<const gl::Point> curve, int subdivisions) -> std::vector<gl::Point>;
  auto calculateIndices
    (
      gsl::span<const gl::Point> vertices,
      unsigned int row_w,
      CurveType type = CurveType::open
    ) -> std::vector<unsigned int>;
}
