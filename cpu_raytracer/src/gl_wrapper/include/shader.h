//
// Created by Jack Glass on 2020-09-21.
//

#pragma once
#include <string>
#include "handle.h"

namespace gl {

  class ShaderProgram;

  class Shader
  {
    friend class ShaderProgram;
  public:
    Shader(std::string  path, GLenum type);

    [[nodiscard]]
    auto path() const -> std::string { return m_path; }
    [[nodiscard]]
    auto type() const -> GLenum { return m_type; }

  private:
    const std::string m_path;
    const GLenum m_type;
    const Handle<Shader> m_handle;

    [[nodiscard]]
    std::string read_src_file() const;
    auto compile(const std::string& source) const -> bool;
  };
}
