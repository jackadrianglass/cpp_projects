//
// Created by Jack Glass on 2020-11-14.
//

#include <glm/glm.hpp>
#include <gsl/span>
#include "point.h"
#include "revolved_surface.h"
#include "3d_surface_algorithms.h"

using namespace mdl;

RevolvedSurface::RevolvedSurface(const Curve& curve)
  : m_curve{&curve}
  , m_vertices{}
  , m_indices{}
  , m_updated{false}
{
  // do nothing
}

auto RevolvedSurface::isUpdated() -> bool
{
  return m_updated;
}

auto RevolvedSurface::surfaceVertices() -> std::vector<gl::Point>
{
  if(!m_updated || !m_curve->isUpdated()){
    m_vertices = calcRevolvedSurface(m_curve->curve(), 6);

    const auto row_width = static_cast<unsigned int>(m_curve->curve().size());
    m_indices = calculateIndices(m_vertices, row_width, CurveType::closed);
    m_updated = true;
  }
  return m_vertices;
}

auto RevolvedSurface::surfaceIndices() -> std::vector<unsigned int>
{
  if(!m_updated || !m_curve->isUpdated()){
    m_vertices = calcRevolvedSurface(m_curve->curve(), 6);
    const auto row_width = static_cast<unsigned int>(m_curve->curve().size());
    m_indices = calculateIndices(m_vertices, row_width, CurveType::closed);
    m_updated = true;
  }
  return m_indices;
}




auto RevolvedSurface::setUpdateFlag() -> void
{
  m_updated = false;
}
