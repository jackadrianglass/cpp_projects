//
// Created by Jack Glass on 2020-09-21.
//

#pragma once
#include <algorithm>
#include <glad/glad.h>

namespace gl {

  template<class glType>
  class Handle
  {
  public:
    Handle();
    explicit Handle(GLenum type);
    ~Handle();

    Handle(const Handle& other) = delete;
    Handle(Handle&& other) noexcept;

    Handle operator=(const Handle& other) = delete;
    [[nodiscard]]
    auto operator=(Handle&& other) noexcept -> Handle&{
      std::swap(m_handleId, other.m_handleId);
      return *this;
    };

    [[nodiscard]]
    operator GLuint() const { return m_handleId; };

    [[nodiscard]]
    auto value() const -> GLuint { return m_handleId; };

  private:
    GLuint m_handleId;
  };

}
