//
// Created by Jack Glass on 2020-09-21.
//

#ifndef MYPROJECT_VERTEX_ARRAY_H
#define MYPROJECT_VERTEX_ARRAY_H

#include "handle.h"

namespace gl{
  class VertexArray
  {
  public:
    VertexArray();
    auto bind() const -> void;

  private:
    const Handle<VertexArray> m_handle;
  };
}


#endif//MYPROJECT_VERTEX_ARRAY_H
