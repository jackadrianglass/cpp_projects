//
// Created by Jack Glass on 2020-11-14.
//

#pragma once
#include "shader_program.h"
#include "line.h"
#include "surface.h"

namespace gl{

  enum class DrawMode {
    wireframe,
    solid
  };

  auto drawPoints(const ShaderProgram& shader, const Line& line) -> void;
  auto drawPoints(const gl::ShaderProgram &shader, const gl::Surface &surface) -> void;

  auto drawLines(const ShaderProgram& shader, const Line& line) -> void;

  auto draw(const ShaderProgram& shader, const Line& line) -> void;
  auto draw(const ShaderProgram& shader, const Surface& surface) -> void;
  auto setDrawMode(DrawMode mode) -> void;

}
