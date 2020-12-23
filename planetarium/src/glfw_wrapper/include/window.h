//
// Created by Jack Glass on 2020-09-29.
//

#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include "glm/vec2.hpp"
#include "callback_interface.h"

namespace glfw {
  class Window;
  struct WindowDeleter {
    void operator() (GLFWwindow* window) const {
      glfwDestroyWindow(window);
    }
  };

  class Window
  {
  protected:
    std::unique_ptr<GLFWwindow, WindowDeleter> m_window;
    std::shared_ptr<CallbackInterface> m_callbacks;
  public:
    Window(int width, int height, const std::string& name);

    auto setCallbacks(std::shared_ptr<CallbackInterface> callbacks) -> void;
    virtual ~Window();

    auto run() -> void;
  };

  [[nodiscard]]
  auto pos_to_gl_coor(glm::vec2 cursor_pos, glm::vec2 screen_dimensions) -> glm::vec2;
}// namespace glfw

