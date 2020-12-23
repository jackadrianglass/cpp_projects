//
// Created by Jack Glass on 2020-11-13.
//

#pragma once
#include <glad/glad.h>
#include <gsl/span>
#include <glm/vec3.hpp>

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
    void uploadIndices(gsl::span<const glm::ivec3> indices, GLenum usage) const;
  };
}
