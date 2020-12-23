//
// Created by Jack Glass on 2020-11-26.
//

#include "callbacks.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "window.h"

Callbacks::Callbacks(glm::vec2 dimensions)
  : m_windowSize{dimensions}
  , m_cursorPos()
  , m_mode(gl::DrawMode::wireframe)
  , m_camera({0, 0, 10}, {0.0f, 0.0f, -90.0f})
  , m_camController(m_camera)
  , m_simpleShader("shaders/simple.vert", "shaders/simple.frag")
  , m_phongShader("shaders/phong.vert", "shaders/phong.frag")
  , m_K{0.5, 0.5, 0.1}
  , m_Ld(1.0f)
  , m_Ls(1.0f)
  , m_La(1.0f)
  , m_alpha{1.0f}
  , m_sun("./textures/sun.jpg", glm::vec2(0))
  , m_earth("./textures/earth.jpg", glm::vec2{10, 2}, 10, 1, true)
  , m_moon("./textures/moon.jpg", glm::vec2{2, 0.5}, 0, 0.25, false)
  , m_stars("./textures/stars.jpg", glm::vec2(0), 0.0f, 100.0f, false)
  , m_animationScale{0.5f}
{
  m_phongShader.uploadUniform("light_pos", glm::vec3{0, 0, 0});
}

auto Callbacks::keyCallback(int key, int scancode, int action, int mods) -> void
{
  m_camController.keyCallback(key, scancode, action, mods);
}

auto Callbacks::mouseButtonCallback(int button, int action, int mods) -> void
{
  m_camController.mouseButtonCallback(button, action, mods);
}

auto Callbacks::cursorPosCallback(glm::vec2 pos) -> void
{
  m_cursorPos = glfw::pos_to_gl_coor(pos, m_windowSize);
  m_camController.cursorPosCallback(m_cursorPos);
}

auto Callbacks::scrollCallback(glm::vec2) -> void
{

}

auto Callbacks::windowSizeCallback(glm::ivec2 size) -> void
{
  m_windowSize = size;
  glViewport(0, 0, static_cast<GLsizei>(m_windowSize.x), static_cast<GLsizei>(m_windowSize.y));
}

auto Callbacks::update() -> void
{
  m_camController.update();
  m_simpleShader.uploadUniform("view", m_camera.viewMatrix());
  m_simpleShader.uploadUniform("projection", m_camera.projectionMatrix(m_windowSize));

  m_phongShader.uploadUniform("cam_pos", m_camera.position());
  m_phongShader.uploadUniform("view", m_camera.viewMatrix());
  m_phongShader.uploadUniform("projection", m_camera.projectionMatrix(m_windowSize));

  static auto time = glfwGetTime();
  auto currentTime = glfwGetTime();

  if(m_animationScale == 0) return;
  auto delta =  m_animationScale * static_cast<float>(currentTime - time);
  time = currentTime;

  m_earth.update(delta);
  m_moon.update(2 * delta);
}

auto Callbacks::draw() -> void
{
  m_stars.draw(m_simpleShader);
  m_sun.draw(m_simpleShader);

  m_phongShader.uploadUniform("K", m_K);
  m_phongShader.uploadUniform("Ld", m_Ld);
  m_phongShader.uploadUniform("Ls", m_Ls);
  m_phongShader.uploadUniform("La", m_La);
  m_phongShader.uploadUniform("alpha", m_alpha);
  m_earth.draw(m_phongShader);
  m_moon.draw(m_phongShader, glm::translate(glm::mat4(1.0f), m_earth.transform().pos()));
}

auto Callbacks::imgui_draw() -> void
{
  ImGui::Begin("Debug Information");
  ImGui::Text("Window size [%f %f]", m_windowSize.x, m_windowSize.y);
  ImGui::Text("Mouse pos [%f %f]", m_cursorPos.x, m_cursorPos.y);

  if(ImGui::Button("Toggle Draw Mode")){
    if(m_mode == gl::DrawMode::solid){
      m_mode = gl::DrawMode::wireframe;
      gl::setDrawMode(m_mode);
    }
    else if(m_mode == gl::DrawMode::wireframe) {
      m_mode = gl::DrawMode::solid;
      gl::setDrawMode(m_mode);
    }
  }

  ImGui::Text("Animation Speed");
  ImGui::SliderFloat("", &m_animationScale, 0.0f, 3.0f);

  ImGui::Text("K Values [%f %f %f]", m_K.x, m_K.y, m_K.z);
  ImGui::SliderFloat("Kd", &m_K.x, 0.0f, 1.0f);
  ImGui::SliderFloat("Ks", &m_K.y, 0.0f, 1.0f);
  ImGui::SliderFloat("Ka", &m_K.z, 0.0f, 1.0f);

  ImGui::Text("Ld Values [%f %f %f]", m_Ld.x, m_Ld.y, m_Ld.z);
  ImGui::SliderFloat("m_Ld.R", &m_Ld.x, 0.0f, 1.0f);
  ImGui::SliderFloat("m_Ld.G", &m_Ld.y, 0.0f, 1.0f);
  ImGui::SliderFloat("m_Ld.B", &m_Ld.z, 0.0f, 1.0f);

  ImGui::Text("Ls Values [%f %f %f]", m_Ls.x, m_Ls.y, m_Ls.z);
  ImGui::SliderFloat("m_Ls.R", &m_Ls.x, 0.0f, 1.0f);
  ImGui::SliderFloat("m_Ls.G", &m_Ls.y, 0.0f, 1.0f);
  ImGui::SliderFloat("m_Ls.B", &m_Ls.z, 0.0f, 1.0f);

  ImGui::Text("La Values [%f %f %f]", m_La.x, m_La.y, m_La.z);
  ImGui::SliderFloat("m_La.R", &m_La.x, 0.0f, 1.0f);
  ImGui::SliderFloat("m_La.G", &m_La.y, 0.0f, 1.0f);
  ImGui::SliderFloat("m_La.B", &m_La.z, 0.0f, 1.0f);

  ImGui::Text("Alpha Value %f", m_alpha);
  ImGui::SliderFloat("alpha", &m_alpha, 0.0f, 10.0f, "%.3f");

  ImGui::End();
}
