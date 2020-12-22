//
// Created by Jack Glass on 2020-09-28.
//

#include "square.h"

using namespace gl::shapes;

auto Square::operator==(const Square &other) const -> bool
{
  return other.m_vertices == m_vertices;
}

auto Square::operator!=(const Square &other) const -> bool
{
  return other.m_vertices != m_vertices;
}