//
// Created by Jack Glass on 2020-09-21.
//

#ifndef MYPROJECT_DEBUG_H
#define MYPROJECT_DEBUG_H

#include <glad/glad.h>

namespace gl {
namespace debug {

  auto output_handler(
      GLenum source,
      GLenum type,
      GLuint id,
      GLenum severity,
      GLsizei,
      const GLchar* message,
      const void*
    ) -> void;

  auto enable() -> void;
}}
#endif//MYPROJECT_DEBUG_H
