//
// Created by Jack Glass on 2020-09-21.
//

#pragma once
#include <glad/glad.h>
#include <gsl/span>
#include "handle.h"

namespace gl {
  class VertexBuffer
  {
  public:
    VertexBuffer();
    auto bind() const -> void;

    template<typename T>
    auto uploadData(gsl::span<const T> data, GLenum usage) const -> void
    {
      bind();
      glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(), usage);
    }

  private:
    Handle<VertexBuffer> m_handle;
  };
}
