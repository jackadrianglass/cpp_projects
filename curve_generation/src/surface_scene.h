//
// Created by Jack Glass on 2020-11-14.
//

#pragma once
#include <array>
#include <memory>

#include "grid.h"
#include "surface.h"
#include "scene.h"
#include "camera.h"

class SurfaceScene : public glfw::Scene
{
  const mdl::Camera& m_camera;
  const glm::vec2& m_screenDimensions;
  const glm::vec2& m_cursorPos;
  const glm::vec2& m_glfwCursorPos;

  glfw::WindowData m_controlsWindow;

  bool m_showGrid;
  gl::Line m_glXyGrid;

  std::array<mdl::Grid, 3> m_grids;
  std::array<std::unique_ptr<gl::Surface>, 3> m_glGrids;

  gl::Line m_controlPoints;

  int m_currentIdx;

public:
  SurfaceScene(const mdl::Camera& cam, const glm::vec2& dim, const glm::vec2& cursor, const glm::vec2& glfwCursor);

  //glm callbacks
  auto keyCallback(int key, int scancode, int action, int mods) -> void override;
  auto mouseButtonCallback(int button, int action, int mods) -> void override;
  auto scrollCallback(double xoffset, double yoffset) -> void override;

  //Window callbacks
  auto update() -> void override;
  auto draw(gl::ShaderProgram& shader) -> void override;
  auto imgui_draw() -> void override;

  auto blockCameraMovements() -> bool override;
};
