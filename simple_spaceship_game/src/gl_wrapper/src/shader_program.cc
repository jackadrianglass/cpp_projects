//
// Created by Jack Glass on 2020-09-21.
//
#include "shader_program.h"

#include <stdexcept>
#include <vector>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

auto ShaderProgram::getId(const std::string& name) const
{
  auto id = glGetUniformLocation(m_handle, name.c_str());
  if(id == -1) throw std::runtime_error("Unable to find uniform with name " + name);
  return id;
}

template <>
auto ShaderProgram::uploadUniform(const std::string& name, float data) const -> void
{
  auto id = getId(name);
  glUniform1f(id, data);
}

template <>
auto ShaderProgram::uploadUniform(const std::string& name, glm::vec2 data) const -> void
{
  auto id = getId(name);
  glUniform2f(id, data.x, data.y);
}

template <>
auto ShaderProgram::uploadUniform(const std::string &name, glm::vec3 data) const -> void
{
  auto id = getId(name);
  glUniform3fv(id, 1, glm::value_ptr(data));
}

template <>
auto ShaderProgram::uploadUniform(const std::string &name, glm::mat2 data) const -> void
{
  auto id = getId(name);
  glUniformMatrix2fv(id, 1, false, glm::value_ptr(data));
}

template <>
auto ShaderProgram::uploadUniform(const std::string &name, glm::mat3 data) const -> void
{
  auto id = getId(name);
  glUniformMatrix3fv(id, 1, false, glm::value_ptr(data));
}

template <>
auto ShaderProgram::uploadUniform(const std::string &name, glm::mat4 data) const -> void
{
  auto id = getId(name);
  glUniformMatrix4fv(id, 1, false, glm::value_ptr(data));
}

template <typename T>
auto ShaderProgram::uploadUniform(const std::string& name, T data) const -> void
{
  throw std::runtime_error("Unable to upload data as there is no method implemented for it");
}