//
// Created by Jack Glass on 2020-09-21.
//

#pragma once
#include "handle.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "buffer_layout.h"

namespace gl{

  class VertexArray
  {
  private:
    const gl::Handle<VertexArray> m_handle;
    std::vector<BufferLayout> m_layouts;

  public:
    VertexArray();
    auto bind() const -> void;
    auto addBuffer(const VertexBuffer& buffer, const BufferLayout& layout) -> void;
  };

}


