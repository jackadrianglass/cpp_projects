//
// Created by Jack Glass on 2020-09-28.
//

#include "square_diamond_scene.h"
#include "squares_and_diamonds.h"

using namespace fractals;

SquareDiamondScene::SquareDiamondScene()
  : m_squares(squares_and_diamonds(max_depth))
  , m_depth(0)
  , m_geom()
{
  auto i = 0;
  for(const auto& square : m_squares){
    auto& g = m_geom[i];
    g.bind();
    g.setColours(square.m_colour, 4);
    g.setVertices(square.m_vertices);
    i++;
  }
}

auto SquareDiamondScene::draw() const -> void
{
  auto i = 0;
  for(const auto& geom : m_geom){
    geom.bind();
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    i++;
    if(i >= 2 * (m_depth + 1)) break;
  }
}

auto SquareDiamondScene::increment_depth() -> void
{
  if(m_depth == max_depth) return;
  m_depth++;
}

auto SquareDiamondScene::decrement_depth() -> void
{
  if(m_depth == 0) return;
  m_depth--;
}