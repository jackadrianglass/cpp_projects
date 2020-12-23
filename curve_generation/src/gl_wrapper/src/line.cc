//
// Created by Jack Glass on 2020-11-14.
//

#include "line.h"

using namespace gl;
using namespace gl;

Line::Line()
  : m_vao()
  , m_vbo()
  , m_numPoints{0}
{
  const auto type_info = std::get<Type<float>>(type_to_gl_type);
  const auto layout = BufferLayout({
                                     { type_info.value, 3, GL_FALSE },
                                     { type_info.value, 3, GL_FALSE }
                                   });

  m_vao.addBuffer(m_vbo, layout);
}

auto Line::bind() const -> void
{
  m_vao.bind();
  m_vbo.bind();
}

auto Line::setPoints(gsl::span<const gl::Point> points) -> void
{
  m_vao.bind();
  m_numPoints = points.size();
  m_vbo.uploadData(points, GL_STATIC_DRAW);
}

auto Line::count() const -> size_t
{
  return m_numPoints;
}