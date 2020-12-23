//
// Created by Jack Glass on 2020-11-11.
//
#include "type_to_gl_map.h"

using namespace gl;
static_assert(sizeof(float) == sizeof(GLfloat),
              "float and GLfloat sizes do not match");

static_assert(std::get<Type<float>>(map).value == GL_FLOAT,
              "float to gl_float does not work");