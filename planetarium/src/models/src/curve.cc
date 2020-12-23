//
// Created by jacka on 2020-11-10.
//

#include <cmath>
#include <spdlog/spdlog.h>
#include "2d_curve_algorithms.h"
#include "curve.h"
#include "colours.h"

using namespace mdl;

Curve::Curve(int subdivisions, Algorithm type)
  : m_ctrlPoints{}
  , m_selectedIdx{-1}
  , m_curveType{type}
  , m_subdivisions{subdivisions}
  , m_updated{false}
{
  // do nothing
}

auto Curve::selectPointCloseTo(glm::vec3 ipoint) -> void
{
  if(m_ctrlPoints.empty()) return;
  if(m_selectedIdx != -1){
    m_ctrlPoints[m_selectedIdx].col = slate_blue;
    m_updated = false;
  }

  m_selectedIdx = 0;
  for(const auto& point : m_ctrlPoints) {
    if(std::abs(point.pos.x - ipoint.x) <= m_closeEnough
      && std::abs(point.pos.y - ipoint.y) <= m_closeEnough
      && std::abs(point.pos.z - ipoint.z) <= m_closeEnough){

      m_ctrlPoints[m_selectedIdx].col = crimson;
      m_updated = false;
      return;
    }
    m_selectedIdx++;
  }
  m_selectedIdx = -1;
}

auto Curve::moveSelected(glm::vec3 point) -> void
{
  if(m_selectedIdx < 0){
    return;
  }
  m_ctrlPoints[m_selectedIdx].pos = point;
  m_updated = false;
}

auto Curve::ctrlPoints() const -> std::vector<gl::Point>
{
  return m_ctrlPoints;
}

auto Curve::havePointSelected() const -> bool
{
  return m_selectedIdx != -1;
}

static constexpr auto zero_point = gl::Point{glm::vec3(0), glm::vec3(0)};
auto Curve::getSelected() const -> gl::Point
{
  if(m_selectedIdx < 0){
    return zero_point;
  }
  return m_ctrlPoints[m_selectedIdx];
}

auto Curve::isUpdated() const -> bool
{
  return m_updated;
}

auto Curve::subdivisions() const -> int
{
  return m_subdivisions;
}

auto Curve::curveType() const -> Algorithm
{
  return m_curveType;
}

auto Curve::curve() const -> std::vector<gl::Point>
{
  if(!m_updated){
    switch(m_curveType){
      case Algorithm::bspline: {
        m_curve = chaikin(m_ctrlPoints, m_subdivisions);
        break;
      }
      case Algorithm::bezier: {
        m_curve = decasteljau(m_ctrlPoints, m_subdivisions);
      }
    }
    m_updated = true;
  }
  return m_curve;
}

auto Curve::addCtrlPoint(glm::vec3 point) -> void
{
  m_ctrlPoints.push_back({point, slate_blue});
  m_updated = false;
}

auto Curve::deleteSelected() -> void
{
  if(m_selectedIdx == -1) return;
  m_ctrlPoints.erase(std::begin(m_ctrlPoints) + m_selectedIdx);
  m_selectedIdx = -1;
  m_updated = false;
}

auto Curve::clear() -> void
{
  m_ctrlPoints.clear();
  m_updated = false;
}

auto Curve::setCurveType(Algorithm type) -> void
{
  m_curveType = type;
  if(m_curveType == Algorithm::bspline && m_subdivisions > 10){
    m_subdivisions = 10;
  }
  m_updated = false;
}

auto Curve::setNumSubdivisions(int num) -> void
{
  if(num == m_subdivisions) return;
  if(num <= 0){
    spdlog::error("Subdivision level is too small {}", num);
    return;
  }
  m_subdivisions = num;
  m_updated = false;
}