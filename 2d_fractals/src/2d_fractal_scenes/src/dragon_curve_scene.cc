//
// Created by Jack Glass on 2020-09-29.
//

#include "dragon_curve_scene.h"
#include "dragon_curve.h"

using namespace fractals;

constexpr auto point_a = glm::vec3{-0.5f, 0.0f, 0.0f};
constexpr auto point_b = glm::vec3{0.5f, 0.0f, 0.0f};

constexpr auto crimson = glm::vec3{0.863f, 0.078f, 0.235f};
constexpr auto violet = glm::vec3{0.498f, 0.0f, 1.0f};

DragonCurveScene::DragonCurveScene()
  : m_num_vertices(0)
  , m_depth(0)
  , m_geom()
{
  recalculate_geometry();
}

auto DragonCurveScene::recalculate_geometry() -> void
{
  const auto vertices = dragon_curve(point_a, point_b, m_depth);

  m_num_vertices = vertices.size();
  m_geom.setVertices(vertices);

  std::vector<glm::vec3> colours{};
  colours.reserve(vertices.size());

  for(auto i = 0; i < vertices.size(); i++){
    if(i % 2 == 0){
      colours.push_back(crimson);
    } else {
      colours.push_back(violet);
    }
  }

  m_geom.setColours(colours);
}

auto DragonCurveScene::draw() const -> void
{
  m_geom.bind();
  glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_num_vertices));
}

auto DragonCurveScene::increment_depth() -> void
{
  if(m_depth == max_depth) return;
  m_depth++;
  recalculate_geometry();
}

auto DragonCurveScene::decrement_depth() -> void
{
  if(m_depth == 0) return;
  m_depth--;
  recalculate_geometry();
}