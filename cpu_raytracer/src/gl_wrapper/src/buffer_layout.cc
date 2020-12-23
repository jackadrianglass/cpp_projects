//
// Created by Jack Glass on 2020-11-11.
//

#include "buffer_layout.h"

#include <utility>

using namespace gl;

BufferLayout::BufferLayout(std::vector<BufferElement> layout)
  : m_elements(std::move(layout))
  , m_stride{0}
{
  for(const auto & element : m_elements){
    m_stride += element.count * gl_type_size.at(element.glType);
  }
}

BufferLayout::BufferLayout()
  : m_elements()
  , m_stride{0}
{}