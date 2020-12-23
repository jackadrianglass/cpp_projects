//
// Created by Jack Glass on 2020-11-14.
//

#include "surface.h"

using namespace gl;

Surface::Surface()
  : m_vao()
  , m_vbo()
  , m_vertCount{0}
  , m_ibo()
  , m_idxCount{0}
{
  const auto type_info = std::get<Type<float>>(type_to_gl_type);
  const auto layout = BufferLayout({
                                     { type_info.value, 3, GL_FALSE },
                                     { type_info.value, 3, GL_FALSE }
                                   });

  m_vao.addBuffer(m_vbo, layout);
}

auto Surface::bind() const -> void
{
  m_vao.bind();
  m_vbo.bind();
  m_ibo.bind();
}

auto Surface::setVertices(gsl::span<const Point> vertices) -> void
{
  m_vao.bind();
  m_vbo.uploadData(vertices, GL_STATIC_DRAW);
  m_vertCount = vertices.size();
}

auto Surface::setIndices(gsl::span<const unsigned int> indices) -> void
{
  m_idxCount = indices.size();
  m_ibo.uploadIndices(indices, GL_STATIC_DRAW);
}

auto Surface::idxCount() const -> size_t
{
  return m_idxCount;
}

auto Surface::vertCount() const -> size_t
{
  return m_vertCount;
}
