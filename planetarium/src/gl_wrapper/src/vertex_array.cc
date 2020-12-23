//
// Created by Jack Glass on 2020-09-21.
//

#include "vertex_array.h"
#include "type_to_gl_map.h"

using namespace gl;

VertexArray::VertexArray()
  : m_handle()
  , m_layouts()
{
  bind();
}

auto VertexArray::bind() const -> void
{
  glBindVertexArray(m_handle);
}

auto VertexArray::addBuffer(const VertexBuffer &buffer, const BufferLayout& layout) -> void
{
  bind();
  buffer.bind();

  auto offset = 0;
  auto idx = 0;

  for(const auto&el : layout.elements()){
    auto type_size = gl_type_size.at(el.glType);
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, el.count, el.glType, el.normalized, layout.stride(), reinterpret_cast<const void *>(offset));
    idx++;
    offset += el.count * type_size;
  }

  m_layouts.push_back(layout);
}

