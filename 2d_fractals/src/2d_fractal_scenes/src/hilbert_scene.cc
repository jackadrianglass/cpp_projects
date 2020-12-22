//
// Created by Jack Glass on 2020-09-30.
//

#include "hilbert_scene.h"
#include "hilbert_curve.h"

using namespace fractals;

constexpr auto green = glm::vec3{0, 1, 0};
constexpr auto purple = glm::vec3{0.5, 0, 0.5};

HilbertScene::HilbertScene()
  : m_num_vertices(0)
  , m_depth(1)
  , m_geom()
{
  recalculate_geometry();
}

auto HilbertScene::recalculate_geometry() -> void
{
  const auto vertices = hilbert_curve(m_depth);
  m_num_vertices = vertices.size();
  m_geom.setVertices(vertices);

  std::vector<glm::vec3> colours{};
  colours.reserve(vertices.size());

  for(auto i = 0; i < vertices.size(); i++){
    if(i % 2 == 0){
      colours.push_back(green);
    }else{
      colours.push_back(purple);
    }
  }
  m_geom.setColours(colours);
}

auto HilbertScene::draw() const -> void
{
  m_geom.bind();
  glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_num_vertices));
}

auto HilbertScene::increment_depth() -> void
{
  if(m_depth >= max_depth + 1) return;
  m_depth++;
  recalculate_geometry();
}

auto HilbertScene::decrement_depth() -> void
{
  if(m_depth <= 1) return;
  m_depth--;
  recalculate_geometry();
}