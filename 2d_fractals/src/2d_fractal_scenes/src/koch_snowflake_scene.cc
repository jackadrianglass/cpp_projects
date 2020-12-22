//
// Created by Jack Glass on 2020-09-29.
//

#include "koch_snowflake_scene.h"
#include "koch_snowflake.h"

using namespace fractals;
using gl::shapes::Triangle;

constexpr auto lime_green = glm::vec3{0.196f, 0.804f, 0.196f};
constexpr auto cyan = glm::vec3{0.0f, 1.0f, 1.0f};

constexpr auto starting_triangle = Triangle{
  glm::vec3{-0.5f, -0.5f, 0.0f},
  glm::vec3{0.5f, -0.5f, 0.0f},
  glm::vec3{0.0f, 0.5f, 0.0f},
  glm::vec3{0.05f, 0.05f, 0.05f}
};

KochSnowflakeScene::KochSnowflakeScene()
  : m_depth(0)
  , m_num_vertices(0)
  , m_geom()
{
  recalculate_geometry();
}

auto KochSnowflakeScene::draw() const -> void
{
  m_geom.bind();
  glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(m_num_vertices));
}

auto KochSnowflakeScene::increment_depth() -> void
{
  if(m_depth == max_depth) return;
  m_depth++;
  recalculate_geometry();
}

auto KochSnowflakeScene::decrement_depth() -> void
{
  if(m_depth == 0) return;
  m_depth--;
  recalculate_geometry();
}

auto KochSnowflakeScene::recalculate_geometry() -> void
{
  const auto vertices = koch_snowflake(starting_triangle, m_depth);
  m_geom.setVertices(vertices);
  m_num_vertices = vertices.size();

  if(m_depth % 2 == 0){
    m_geom.setColours(cyan, vertices.size());
  } else{
    m_geom.setColours(lime_green, vertices.size());
  }
}