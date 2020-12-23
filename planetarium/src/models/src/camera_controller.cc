//
// Created by Jack Glass on 2020-11-27.
//

#include "camera_controller.h"
#include <GLFW/glfw3.h>

using namespace mdl;

CameraController::CameraController(mdl::Camera &camera)
  : m_camera(camera)
  , m_backForward{0}
  , m_leftRight{0}
  , m_upDown{0}
  , m_isPressed{false}
  , m_prevCursorPos()
  , m_cursorPos()
{

}

auto CameraController::keyCallback(int key, [[maybe_unused]]int scancode, int action, [[maybe_unused]]int mods) -> void
{
  if(action == GLFW_PRESS){
    switch(key) {
    case GLFW_KEY_W: {
      m_backForward++;
      break;
    }
    case GLFW_KEY_S: {
      m_backForward--;
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
    case GLFW_KEY_SPACE: {
      if(mods == GLFW_MOD_SHIFT){
        m_upDown--;
      } else {
        m_upDown++;
      }
      break;
    }
    case GLFW_KEY_LEFT_SHIFT: {
      m_upDown *= -1;
      break;
    }
    default: break;
    }
  }

  if(action == GLFW_RELEASE){
    switch(key) {
    case GLFW_KEY_W: {
      m_backForward--;
      break;
    }
    case GLFW_KEY_S: {
      m_backForward++;
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
    case GLFW_KEY_SPACE: {
      if(mods == GLFW_MOD_SHIFT){
        m_upDown++;
      } else {
        m_upDown--;
      }
      break;
    }
    case GLFW_KEY_LEFT_SHIFT: {
      m_upDown *= -1;
      break;
    }
    default: break;
    }
  }
}

auto CameraController::mouseButtonCallback(int button, int action, [[maybe_unused]]int mods) -> void
{
  if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT){
    m_isPressed = true;
  }
  if(action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_RIGHT){
    m_isPressed = false;
  }
}

auto CameraController::cursorPosCallback(glm::vec2 pos) -> void
{
  m_prevCursorPos = m_cursorPos;
  m_cursorPos = pos;
}

auto CameraController::update() -> void
{
  // update the camera
  if(m_isPressed){
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

  if(m_backForward < 0) {
    m_camera.move(mdl::Direction::backward, delta);
  } else if(m_backForward > 0){
    m_camera.move(mdl::Direction::forward, delta);
  }

  if(m_upDown < 0) {
    m_camera.move(mdl::Direction::down, delta);
  } else if(m_upDown > 0) {
    m_camera.move(mdl::Direction::up, delta);
  }
}