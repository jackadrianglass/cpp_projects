//
// Created by Jack Glass on 2020-11-27.
//

#pragma once

#include "camera.h"

namespace mdl {
  class CameraController
  {
  private:
    mdl::Camera &m_camera;
    int m_backForward;
    int m_leftRight;
    int m_upDown;

    bool m_isPressed;
    glm::vec2 m_prevCursorPos;
    glm::vec2 m_cursorPos;

  public:
    CameraController(mdl::Camera &camera);

    auto keyCallback(int key, int scancode, int action, int mods) -> void;
    auto mouseButtonCallback(int button, int action, int mods) -> void;
    auto cursorPosCallback(glm::vec2 pos) -> void;
    void update();
  };
}