//
// Created by jacka on 2020-11-10.
//

#include "imgui.h"
#include "spdlog/spdlog.h"

#include "callbacks.h"

Callbacks::Callbacks(int width, int height)
  : m_isPressed{false}
  , m_cursorPos()
  , m_prevCursorPos()
  , m_glfwCursorPos()
  , m_infoWindow()
  , m_screenDimensions(width, height)
  , m_camera({0.0f, 0.0f, 3.0f}, Orientation(0.0f, 0.0f, -90.0f))
  , m_shader("./shaders/test.vert", "./shaders/test.frag")
  , m_mode{gl::DrawMode::solid}
  , m_scene{which::surface}
  , m_curveScene(m_camera, m_screenDimensions, m_cursorPos, m_glfwCursorPos)
  , m_surfaceScene(m_camera, m_screenDimensions, m_cursorPos, m_glfwCursorPos)
{
}

auto Callbacks::keyCallback(int key, int scancode, int action, int mods) -> void
{

  if(action == GLFW_PRESS){
    switch(key) {
      case GLFW_KEY_W: {
        m_upDown++;
        break;
      }
      case GLFW_KEY_S: {
        m_upDown--;
        break;
      }
      case GLFW_KEY_A: {
        m_leftRight--;
        break;
      }
      case GLFW_KEY_D: {
        m_leftRight++;
        break;
      }
      default: break;
    }
  }

  if(action == GLFW_RELEASE){
    switch(key) {
      case GLFW_KEY_W: {
        m_upDown--;
        break;
      }
      case GLFW_KEY_S: {
        m_upDown++;
        break;
      }
      case GLFW_KEY_A: {
        m_leftRight++;
        break;
      }
      case GLFW_KEY_D: {
        m_leftRight--;
        break;
      }
      default: break;
    }
  }

  if(m_scene == which::curve) {
    m_curveScene.keyCallback(key, scancode, action, mods);
  } else {
    m_surfaceScene.keyCallback(key, scancode, action, mods);
  }
}

auto Callbacks::mouseButtonCallback(int button, int action, int mods) -> void
{
  if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT){
    m_isPressed = true;
  }
  if(action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT){
    m_isPressed = false;
  }

  if(m_scene == which::curve) {
    m_curveScene.mouseButtonCallback(button, action, mods);
  } else {
    m_surfaceScene.mouseButtonCallback(button, action, mods);
  }
}

auto Callbacks::cursorPosCallback(double x_pos, double y_pos) -> void
{
  m_prevCursorPos = m_cursorPos;
  m_glfwCursorPos = {x_pos, y_pos};
  m_cursorPos = glfw::pos_to_gl_coor(m_glfwCursorPos, m_screenDimensions);
}

//auto Callbacks::scrollCallback(double xoffset, double yoffset) -> void
auto Callbacks::scrollCallback(double, double) -> void {}

auto Callbacks::windowSizeCallback(int width, int height) -> void
{
  m_screenDimensions = {width, height};
  glViewport(0, 0, width, height);
}

auto Callbacks::draw() -> void
{
  // update
  update();

  const auto& projection = m_camera.projectionMatrix(m_screenDimensions);
  const auto& view = m_camera.viewMatrix();
  m_shader.uploadUniform("projection", projection);
  m_shader.uploadUniform("view", view);

  if(m_scene == which::curve) {
    m_curveScene.draw(m_shader);
  } else {
    m_surfaceScene.draw(m_shader);
  }
}

auto Callbacks::update() -> void
{
  bool sceneBlock = false;
  if(m_scene == which::curve) {
    m_curveScene.update();
    sceneBlock = m_curveScene.blockCameraMovements();
  } else {
    m_surfaceScene.update();
    sceneBlock = m_surfaceScene.blockCameraMovements();
  }

  if(m_isPressed && !(sceneBlock || blockCameraMovements())){
    const auto deltaPos = m_prevCursorPos - m_cursorPos;
    m_camera.rotate(deltaPos);
  }

  static auto time = glfwGetTime();
  auto currentTime = glfwGetTime();
  auto delta = currentTime - time;
  time = currentTime;
  if(m_leftRight < 0){
    m_camera.move(mdl::Direction::left, delta);
  } else if(m_leftRight > 0){
    m_camera.move(mdl::Direction::right, delta);
  }

  if(m_upDown < 0) {
    m_camera.move(mdl::Direction::backward, delta);
  } else if(m_upDown > 0){
    m_camera.move(mdl::Direction::forward, delta);
  }
}

auto Callbacks::imgui_draw() -> void
{

  ImGui::Begin("Window Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Screen Dimensions [%f, %f]", m_screenDimensions.x, m_screenDimensions.y);
  ImGui::Text("Mouse Position [%f, %f]", m_cursorPos.x, m_cursorPos.y);

  const auto& cam = m_camera.position();
  ImGui::Text("Camera Position [%f, %f, %f]", cam.x, cam.y, cam.z);

  auto pos = ImGui::GetWindowPos();
  auto size = ImGui::GetWindowSize();
  m_infoWindow = {{pos.x, pos.y}, {size.x, size.y}};

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

  if(ImGui::Button("Toggle Scene")){
    if(m_scene == which::curve){
      m_scene = which::surface;
    }
    else{
      m_scene = which::curve;
    }
  }
  ImGui::End();
  if(m_scene == which::curve) {
    m_curveScene.imgui_draw();
  } else {
    m_surfaceScene.imgui_draw();
  }
}

bool Callbacks::blockCameraMovements()
{
  auto result = glfw::intersects(m_infoWindow, m_glfwCursorPos);
  return result;
}
