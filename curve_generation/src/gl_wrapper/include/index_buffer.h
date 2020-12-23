//
// Created by Jack Glass on 2020-11-13.
//

#pragma once
#include <glad/glad.h>
#include <gsl/span>

#include "handle.h"

namespace gl {
  class IndexBuffer
  {
  private:
    Handle<IndexBuffer> m_handle;

  public:
    IndexBuffer();
    auto bind() const -> void;

    auto uploadIndices(gsl::span<const unsigned int> indices, GLenum usage) const -> void;
  };
}
