//
// Created by Jack Glass on 2020-09-23.
//

#include "debug.h"

#include <string>
#include <map>

#include <spdlog/spdlog.h>

auto gl::debug::output_handler(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei,
  const GLchar* message,
  const void*
  ) -> void
{
  using namespace std::literals;
  static const auto sources = std::map<GLenum, std::string> {
    {GL_DEBUG_SOURCE_API, "API"},
    {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System"},
    {GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler"},
    {GL_DEBUG_SOURCE_THIRD_PARTY, "Third Party"},
    {GL_DEBUG_SOURCE_APPLICATION, "Window"},
    {GL_DEBUG_SOURCE_OTHER, "Other"},
  };
  static const auto types = std::map<GLenum, std::string> {
    {GL_DEBUG_TYPE_ERROR, "Error"},
    {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behaviour"},
    {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined Behaviour"},
    {GL_DEBUG_TYPE_PORTABILITY, "Portability"},
    {GL_DEBUG_TYPE_PERFORMANCE, "Performance"},
    {GL_DEBUG_TYPE_MARKER, "Marker"},
    {GL_DEBUG_TYPE_PUSH_GROUP, "Push Group"},
    {GL_DEBUG_TYPE_POP_GROUP, "Pop Group"},
    {GL_DEBUG_TYPE_OTHER, "Other"},
  };

  const auto find_str = [](const auto& map, auto key){
       const auto result = map.find(key);
       if(result != map.end()){
         return result->second;
       } else {
         return "Unknown"s;
       }
  };
  const auto source_str = find_str(sources, source);
  const auto type_str = find_str(types, type);

  const auto format = "[OPENGL][{}] {} #{} -- {}: {}"s;

  switch(severity){
    case GL_DEBUG_SEVERITY_HIGH:
      spdlog::error(format.c_str(), source_str, "high", id, type_str, message);
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      spdlog::warn(format.c_str(), source_str, "medium", id, type_str, message);
      break;
    case GL_DEBUG_SEVERITY_LOW:
      spdlog::warn(format.c_str(), source_str, "low", id, type_str, message);
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
//      spdlog::info(format.c_str(), source_str, "notification", id, type_str, message);
      break;
    default:
      // Don't care about the rest
      break;
  }
}

auto gl::debug::enable() -> void
{
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if(flags & GL_CONTEXT_FLAG_DEBUG_BIT){
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl::debug::output_handler, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
    spdlog::info("Enabling opengl debug messages");
  } else {
    spdlog::warn("Unable to enable opengl debug messages");
  }
}

