//
// Created by Jack Glass on 2020-09-28.
//

#include "serpinsky_scene.h"
#include "serpinsky_triangle.h"

using namespace fractals;

SerpinskyScene::SerpinskyScene()
  : m_current_geometry()
  , m_current_depth(0)
{
  recalculate_geometry();
}

auto SerpinskyScene::draw() const -> void
{
  m_current_geometry.bind();
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_number_of_vertices));
}

auto SerpinskyScene::increment_depth() -> void
{
  if(m_current_depth == max_depth) return;
  m_current_depth++;
  recalculate_geometry();
}

auto SerpinskyScene::decrement_depth() -> void
{
  if(m_current_depth == 0) return;
  m_current_depth--;
  recalculate_geometry();
}

auto SerpinskyScene::recalculate_geometry() -> void
{
  const auto geom = serpinsky_triangle(m_current_depth);
  m_current_geometry.setVertices(geom.vertices);
  m_current_geometry.setColours(geom.colours);
  m_number_of_vertices = geom.vertices.size();
}