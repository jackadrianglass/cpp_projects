//
// Created by Jack Glass on 2020-11-14.
//

#include "renderer.h"
auto gl::drawPoints(const gl::ShaderProgram &shader, const gl::Line &line) -> void
{
  shader.bind();
  line.bind();
  glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(line.count()));
}

auto gl::drawPoints(const gl::ShaderProgram& shader, const gl::Surface& surface) -> void
{
  shader.bind();
  glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(surface.vertCount()));
}

auto gl::drawLines(const gl::ShaderProgram &shader, const gl::Line &line) -> void
{
  shader.bind();
  line.bind();
  glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(line.count()));
}

auto gl::draw(const gl::ShaderProgram &shader, const gl::Line &line) -> void
{
  shader.bind();
  line.bind();
  glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(line.count()));
}

auto gl::draw(const gl::ShaderProgram &shader, const gl::Surface &surface) -> void
{
  shader.bind();
  surface.bind();
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(surface.idxCount()), GL_UNSIGNED_INT, nullptr);
}

//todo add function to just draw points of a surface

auto gl::setDrawMode(gl::DrawMode mode) -> void
{
  if(mode == DrawMode::solid){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  else if(mode == DrawMode::wireframe){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
}
