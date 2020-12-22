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

  private:
    const Handle<ShaderProgram> m_handle;

    Shader m_vertex;
    Shader m_fragment;

    auto checkLinkStatus() const -> bool;
  };
}


#endif//MYPROJECT_SHADER_PROGRAM_H
