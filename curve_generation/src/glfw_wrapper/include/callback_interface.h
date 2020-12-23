//
// Created by Jack Glass on 2020-11-13.
//

#pragma once
#include "glm/vec2.hpp"

namespace glfw {
  class CallbackInterface {
  public:
    virtual ~CallbackInterface() = default;

    //glm callbacks
    virtual auto keyCallback(int key, int scancode, int action, int mods) -> void = 0;
    virtual auto mouseButtonCallback(int button, int action, int mods) -> void = 0;
    virtual auto cursorPosCallback(double xpos, double ypos) -> void = 0;
    virtual auto scrollCallback(double xoffset, double yoffset) -> void = 0;
    virtual auto windowSizeCallback(int width, int height) -> void = 0;

    //Window callbacks
    virtual auto update() -> void = 0;
    virtual auto draw() -> void = 0;
    virtual auto imgui_draw() -> void = 0;
  };
}// namespace glfw