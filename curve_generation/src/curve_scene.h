//
// Created by Jack Glass on 2020-11-14.
//

#pragma once
#include "scene.h"

#include "line.h"
#include "surface.h"

#include "grid_outline.h"
#include "curve.h"
#include "revolved_surface.h"
#include "camera.h"


class CurveScene : public glfw::Scene
{
private:
  const mdl::Camera& m_camera;
  const glm::vec2& m_screenDimensions;
  const glm::vec2& m_cursorPos;
  const glm::vec2& m_glfwCursorPos;

  glfw::WindowData m_infoWindow;
  glfw::WindowData m_controlsWindow;

  // things to draw
  bool m_isPressed;
  bool m_showGrid;
  mdl::GridOutline m_grid;
  gl::Line m_glGrid;

  mdl::Curve m_curve;
  gl::Line m_glPoints;
  gl::Line m_glCurve;

  bool m_drawSurface;
  mdl::RevolvedSurface m_surface;
  gl::Surface m_glSurface;

public:
  CurveScene(const mdl::Camera& cam, const glm::vec2& dim, const glm::vec2& cursor, const glm::vec2& glfwCursor);
  //glm callbacks
  auto keyCallback(int key, int scancode, int action, int mods) -> void override;
  auto mouseButtonCallback(int button, int action, int mods) -> void override;
  auto scrollCallback(double xoffset, double yoffset) -> void override;

  //Window callbacks
  auto update() -> void override;
  auto draw(gl::ShaderProgram& shader) -> void override;
  auto imgui_draw() -> void override;

  auto blockCameraMovements() -> bool override;

private:
  void toggleCurveType();
};
