//
// Created by Jack Glass on 2020-11-14.
//

#pragma once

#include <point.h>
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"


namespace gl {
  class Surface
  {
  private:
    VertexArray   m_vao;
    VertexBuffer  m_vbo;
    size_t        m_vertCount;
    IndexBuffer   m_ibo;
    size_t        m_idxCount;

  public:
    Surface();
    auto bind() const -> void;
    auto setVertices(gsl::span<const Point> vertices) -> void;
    auto setIndices(gsl::span<const unsigned int> indices) -> void;

    [[nodiscard]]
    auto idxCount() const -> size_t;

    [[nodiscard]]
    auto vertCount() const -> size_t;
  };
}
