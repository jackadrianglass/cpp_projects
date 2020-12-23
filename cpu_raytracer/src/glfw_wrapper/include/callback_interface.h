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
    virtual auto cursorPosCallback(glm::vec2 pos) -> void = 0;
    virtual auto scrollCallback(glm::vec2 offset) -> void = 0;
    virtual auto windowSizeCallback(glm::ivec2 size) -> void = 0;

    //Window callbacks
    virtual auto update() -> void = 0;
    virtual auto draw() -> void = 0;
    virtual auto imgui_draw() -> void = 0;
  };
}// namespace glfw