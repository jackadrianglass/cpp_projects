//
// Created by Jack Glass on 2020-09-21.
//

#include "shader_program.h"

#include <stdexcept>
#include <vector>
#include <spdlog/spdlog.h>

using namespace gl;

ShaderProgram::ShaderProgram(std::string vertex_path, std::string fragment_path)
  : m_handle()
  , m_vertex(std::move(vertex_path), GL_VERTEX_SHADER)
  , m_fragment(std::move(fragment_path), GL_FRAGMENT_SHADER)
{
  attach(m_vertex);
  attach(m_fragment);
  glLinkProgram(m_handle);

  if(!checkLinkStatus()){
    throw std::runtime_error("Shader failed to link");
  }
}

auto ShaderProgram::use() const -> void
{
  glUseProgram(m_handle);
}

auto ShaderProgram::attach(Shader& s) -> void
{
  glAttachShader(m_handle, s.m_handle);
}

auto ShaderProgram::checkLinkStatus() const -> bool
{
  GLint success;
  glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
  if(!success) {
    GLint log_length;
    glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<char> log(static_cast<size_t>(log_length));
    glGetProgramInfoLog(m_handle, log_length, nullptr, log.data());

    spdlog::error("SHADER PROGRAM linking {} + {}:\n{}",
      m_vertex.path(), m_fragment.path(), log.data());
    return false;
  }
  spdlog::info("SHADER PROGRAM linking successful {} + {}",
    m_vertex.path(), m_fragment.path());
  return true;
}
