//
// Created by Jack Glass on 2020-11-26.
//

#pragma once

#include <vector>
#include <glm/vec2.hpp>

#include "callback_interface.h"
#include "shader_program.h"
#include "camera.h"
#include "camera_controller.h"
#include "renderer.h"
#include "planetoid.h"

class Callbacks : public glfw::CallbackInterface
{
private:
  glm::vec2 m_windowSize;
  glm::vec2 m_cursorPos;
  gl::DrawMode m_mode;

  mdl::Camera m_camera;
  mdl::CameraController m_camController;

  gl::ShaderProgram m_simpleShader;
  gl::ShaderProgram m_phongShader;
  glm::vec3 m_K;
  glm::vec3 m_Ld;
  glm::vec3 m_Ls;
  glm::vec3 m_La;
  float m_alpha;

  Planetoid m_sun;
  Planetoid m_earth;
  Planetoid m_moon;
  Planetoid m_stars;

  float m_animationScale;

public:
  explicit Callbacks(glm::vec2 dimensions);
  //glm callbacks
  auto keyCallback(int key, int scancode, int action, int mods) -> void override;
  auto mouseButtonCallback(int button, int action, int mods) -> void override;
  auto cursorPosCallback(glm::vec2 pos) -> void override;
  auto scrollCallback(glm::vec2 size) -> void override;
  auto windowSizeCallback(glm::ivec2 size) -> void override;

  //Window callbacks
  auto update() -> void override;
  auto draw() -> void override;
  auto imgui_draw() -> void override;
};
