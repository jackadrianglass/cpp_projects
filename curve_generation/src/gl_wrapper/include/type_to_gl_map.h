//
// Created by jacka on 2020-11-10.
//

#pragma once
#include <tuple>
#include <map>
#include <stdexcept>
#include <glad/glad.h>

namespace gl {
  static const auto gl_type_size = std::map<unsigned int, size_t>{
    {GL_FLOAT, sizeof(GLfloat)},
    {GL_INT, sizeof(GLint)},
    {GL_UNSIGNED_INT, sizeof(GLuint)}
  };

  template<typename T>
  struct Type
  {
    unsigned int value; //gl enum value
    size_t size; //in bytes
  };

  static constexpr auto type_to_gl_type = std::tuple{
    Type<float>{ GL_FLOAT, sizeof(GLfloat) },
    Type<int>{ GL_INT,  sizeof(GLint) },
    Type<unsigned int>{ GL_UNSIGNED_INT, sizeof(GLuint)}
    //todo add more as needed
   };
}
