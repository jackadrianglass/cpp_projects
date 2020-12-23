//
// Created by Jack Glass on 2020-09-21.
//

#include "vertex_buffer.h"
using namespace gl;

VertexBuffer::VertexBuffer()
  : m_handle()
{}

auto VertexBuffer::bind() const -> void
{
  glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}
