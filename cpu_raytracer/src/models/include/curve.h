//
// Created by jacka on 2020-11-10.
//
#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "point.h"

namespace mdl {

  enum class Algorithm {
    bezier,
    bspline
  };

  class Curve
  {
  private:
    static constexpr auto m_closeEnough = 0.1f;

    std::vector<gl::Point> m_ctrlPoints;
    int m_selectedIdx;

    Algorithm m_curveType;
    int m_subdivisions;
    mutable bool m_updated;
    mutable std::vector<gl::Point> m_curve{};

  public:
    explicit Curve(int subdivisions, Algorithm type = Algorithm::bspline);

    // Returns true if one is found
    auto selectPointCloseTo(glm::vec3 point) -> void;

    // Returns true if anything is isUpdated
    auto moveSelected(glm::vec3 point) -> void;

    //getters
    [[nodiscard]]
    auto ctrlPoints() const -> std::vector<gl::Point>;
    [[nodiscard]]
    auto havePointSelected() const -> bool;
    [[nodiscard]]
    auto getSelected() const -> gl::Point;
    [[nodiscard]]
    auto isUpdated() const -> bool;
    [[nodiscard]]
    auto subdivisions() const -> int;

    [[nodiscard]]
    auto curveType() const -> Algorithm;
    [[nodiscard]]
    auto curve() const -> std::vector<gl::Point>;

    //setters
    auto addCtrlPoint(glm::vec3 point) -> void;
    auto deleteSelected() -> void;
    auto clear() -> void;

    auto setCurveType(Algorithm type) -> void;
    auto setNumSubdivisions(int num) -> void;
  };
}// namespace mdl
