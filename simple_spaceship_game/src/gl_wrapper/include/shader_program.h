//
// Created by Jack Glass on 2020-09-21.
//

#ifndef MYPROJECT_SHADER_PROGRAM_H
#define MYPROJECT_SHADER_PROGRAM_H


#include <string>
#include "handle.h"
#include "shader.h"

namespace gl {
  class ShaderProgram
  {
  public:
    ShaderProgram(std::string vertex_path, std::string fragment_path);

    auto use() const -> void;
    auto attach(Shader& s) -> void;

    template<typename T>
    auto uploadUniform(const std::string& name, T data) const -> void;

  private:
    const Handle<ShaderProgram> m_handle;

    Shader m_vertex;
    Shader m_fragment;

    [[nodiscard]]
    auto checkLinkStatus() const -> bool;
    [[nodiscard]]
    auto getId(const std::string& name) const;
  };
}


#endif//MYPROJECT_SHADER_PROGRAM_H
