//
// Created by Jack Glass on 2020-11-11.
//

#pragma once
#include <vector>
#include "type_to_gl_map.h"

namespace gl {
  struct BufferElement
  {
    unsigned int glType;
    unsigned int count;
    unsigned char normalized;//either GL_TRUE or GL_False
  };

  class BufferLayout
  {
  private:
    std::vector<BufferElement> m_elements;
    unsigned int m_stride;
  public:
    explicit BufferLayout(std::vector<BufferElement> layout);
    BufferLayout();

    template<typename T>
    auto push(unsigned int count) -> void
    {
      static_assert(std::get<T>(type_to_gl_type), "Have not mapped this type yet");
      auto type = std::get<T>(type_to_gl_type);
      m_elements.push_back({type.value, type.size, count, GL_FALSE});
      m_stride += type.size * count;
    }

    //getters
    [[nodiscard]]
    auto elements() const { return m_elements; }
    [[nodiscard]]
    auto stride() const { return m_stride; }
  };
}