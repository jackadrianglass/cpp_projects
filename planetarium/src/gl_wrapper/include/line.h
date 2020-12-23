//
// Created by Jack Glass on 2020-11-14.
//

#pragma once
#include <gsl/span>
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "point.h"

namespace gl {
  class Line
  {
  private:
    VertexArray  m_vao;
    VertexBuffer m_vbo;
    size_t       m_numPoints;

  public:
    Line();
    auto bind() const -> void;
    auto setPoints(gsl::span<const gl::Point> points) -> void;

    [[nodiscard]]
    auto count() const -> size_t;
  };
}
