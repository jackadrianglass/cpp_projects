//
// Created by Jack Glass on 2020-11-14.
//

#pragma once

#include <glm/glm.hpp>
#include "shader_program.h"
namespace glfw{

  struct WindowData {
    glm::vec2 pos;
    glm::vec2 size;
  };

  class Scene
  {
  public:
    virtual ~Scene() = default;

    //glm callbacks
    virtual auto keyCallback(int key, int scancode, int action, int mods) -> void = 0;
    virtual auto mouseButtonCallback(int button, int action, int mods) -> void = 0;
    virtual auto scrollCallback(double xoffset, double yoffset) -> void = 0;

    //Window callbacks
    virtual auto update() -> void = 0;
    virtual auto draw(gl::ShaderProgram& shader) -> void = 0;
    virtual auto imgui_draw() -> void = 0;

    virtual auto blockCameraMovements() -> bool = 0;
  };

  bool intersects(const WindowData &window, glm::vec2 pos);
  }
