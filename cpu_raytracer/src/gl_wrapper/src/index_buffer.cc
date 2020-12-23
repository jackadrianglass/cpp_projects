//
// Created by Jack Glass on 2020-11-13.
//

#include <glm/glm.hpp>
#include "index_buffer.h"

using namespace gl;
IndexBuffer::IndexBuffer()
  : m_handle()
{
}

auto IndexBuffer::bind() const -> void
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
}

auto IndexBuffer::uploadIndices(gsl::span<const unsigned int> indices, GLenum usage) const -> void
{
  bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);
}

auto IndexBuffer::uploadIndices(gsl::span<const glm::ivec3> indices, GLenum usage) const -> void
{
  bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::ivec3), indices.data(), usage);
}
