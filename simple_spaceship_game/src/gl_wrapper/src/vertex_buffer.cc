//
// Created by Jack Glass on 2020-09-21.
//

#include "vertex_buffer.h"

using namespace gl;

VertexBuffer::VertexBuffer(GLuint index, GLint size, GLenum data_type)
  : m_handle()
{
  bind();
  glVertexAttribPointer(index, size, data_type, false, 0, nullptr);
  glEnableVertexAttribArray(index);
}

auto VertexBuffer::bind() const -> void
{
  glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

auto VertexBuffer::uploadData(GLsizeiptr size, const void *data, GLenum usage) const -> void
{
  bind();
  glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}