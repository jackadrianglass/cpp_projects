//
// Created by Jack Glass on 2020-09-21.
//

#include "shader.h"
#include <spdlog/spdlog.h>

#include <stdexcept>
#include <fstream>
#include <sstream>

using namespace gl;

Shader::Shader(std::string path, GLenum type)
  : m_path(std::move(path))
  , m_type(type)
  , m_handle(type)
{
  const auto src_code = read_src_file();
  if(!compile(src_code)) {
    throw std::runtime_error("Shader failed to compile. See error log");
  }
}

auto Shader::read_src_file() const -> std::string
{
  std::ifstream file{};
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    file.open(m_path);
    std::stringstream source_stream{};

    source_stream << file.rdbuf();
    file.close();
    return source_stream.str();
  }
  catch(std::ifstream::failure &e) {
    spdlog::error("Shader reading {}:\n{}", m_path, e.code().message());
    throw std::runtime_error("Unable to read shader file");
  }
}

auto Shader::compile(const std::string& source) const -> bool
{
  const GLchar* src_ptr{source.c_str()};
  glShaderSource(m_handle, 1, &src_ptr, nullptr);
  glCompileShader(m_handle);

  GLint success;
  glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);

  if(success == 0) {
    GLint log_length;
    glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<char> log(static_cast<size_t>(log_length));
    glGetShaderInfoLog(m_handle, log_length, nullptr, log.data());

    spdlog::error("SHADER compiling error {}:\n{}", source, log.data());
    return false;
  }
  spdlog::info("SHADER compilation successful {}", m_path);
  return true;
}