//
// Created by Jack Glass on 2020-09-21.
//

#ifndef MYPROJECT_VERTEX_BUFFER_H
#define MYPROJECT_VERTEX_BUFFER_H

#include <glad/glad.h>
#include "handle.h"

namespace gl {
  class VertexBuffer
  {
  public:
    VertexBuffer(GLuint index, GLint size, GLenum data_type);
    auto bind() const -> void;
    auto uploadData(GLsizeiptr size, const void* data, GLenum usage) const -> void;

  private:
    const Handle<VertexBuffer> m_handle;
  };

}


#endif//MYPROJECT_VERTEX_BUFFER_H
