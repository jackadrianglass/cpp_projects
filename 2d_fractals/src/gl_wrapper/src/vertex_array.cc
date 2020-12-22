//
// Created by Jack Glass on 2020-09-21.
//

#include "vertex_array.h"

using namespace gl;

VertexArray::VertexArray()
  : m_handle()
{
  bind();
}

auto VertexArray::bind() const -> void
{
  glBindVertexArray(m_handle);
}