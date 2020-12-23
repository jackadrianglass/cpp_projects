//
// Created by Jack Glass on 2020-10-15.
//

#ifndef MYPROJECT_TEXTURE_H
#define MYPROJECT_TEXTURE_H
#include <string>
#include <glm/vec2.hpp>
#include "handle.h"

namespace gl {
  class Texture
  {
  private:
    gl::Handle<Texture> m_handle;
    const std::string m_path;
    const GLint m_interpolation;

    int m_width{};
    int m_height{};

  public:
    Texture(std::string path, GLint interpolation);

    [[nodiscard]]
    auto path() const { return m_path; }

    [[nodiscard]]
    auto interpolation() const { return m_interpolation; }

    [[nodiscard]]
    auto dimensions() const -> glm::ivec2 { return {m_width, m_height};}

    auto bind() const -> void;
    auto unbind() const -> void;
  };
  }


#endif//MYPROJECT_TEXTURE_H
