//
// Created by jacka on 2020-11-10.
//

#pragma once
//3rd party
#include <glm/vec2.hpp>
#include <renderer.h>

//gl
#include "line.h"
#include "surface.h"
#include "shader_program.h"

//models
#include "camera.h"
#include "curve.h"
#include "grid_outline.h"
#include "revolved_surface.h"

//window
#include "window.h"
#include "callback_interface.h"

#include "curve_scene.h"
#include "surface_scene.h"

enum class which {
  curve,
  surface
};

class Callbacks : public glfw::CallbackInterface
{
private:
  //window related things
  bool m_isPressed;
  glm::vec2 m_cursorPos;
  glm::vec2 m_glfwCursorPos;
  glm::vec2 m_prevCursorPos;
  glm::vec2 m_screenDimensions;
  glfw::WindowData m_infoWindow;

  int m_leftRight{0};
  int m_upDown{0};

  // things to draw with
  mdl::Camera m_camera;
  gl::ShaderProgram m_shader;
  gl::DrawMode m_mode;


  // things to draw
  which m_scene;
  CurveScene m_curveScene;
  SurfaceScene m_surfaceScene;


public:
  Callbacks(int width, int height);
  auto keyCallback(int key, int scancode, int action, int mods) -> void override;
  auto mouseButtonCallback(int button, int action, int mods) -> void override;
  auto cursorPosCallback(double xoffset, double yoffset) -> void override;
  auto scrollCallback(double xoffset, double yoffset) -> void override;
  auto windowSizeCallback(int width, int height) -> void override;

  auto update() -> void override;
  auto draw() -> void override;
  auto imgui_draw() -> void override;
  bool blockCameraMovements();
};
