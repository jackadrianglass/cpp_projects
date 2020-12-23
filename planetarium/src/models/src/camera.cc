//
// Created by Jack Glass on 2020-11-12.
//

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

using namespace mdl;

Camera::Camera(glm::vec3 pos, CameraOrientation orientation, CameraSettings settings)
  : settings(settings)
  , m_pos(pos)
  , m_orient{orientation}
{
}

auto Camera::viewMatrix() const -> glm::mat4
{
  if(m_updateView){
    m_view = glm::lookAt(m_pos, m_pos + m_orient.front(), m_orient.up());
    m_updateView = false;
  }
  return m_view;
}

auto Camera::projectionMatrix(glm::vec2 windowDimensions) const -> glm::mat4
{
  auto whRatio = windowDimensions.x / windowDimensions.y;
  if(m_updateProjection || whRatio != m_whRatio){
    m_whRatio = whRatio;
    m_projection = glm::perspective(glm::radians(settings.fov),
                                    m_whRatio, settings.near, settings.far);
    m_updateProjection = false;
  }
  return m_projection;
}

auto Camera::move(Direction direction, double deltaTime) -> void
{
  float vel = settings.speed * static_cast<float>(deltaTime);
  switch(direction) {
    case Direction::forward: {
      m_pos += m_orient.front() * vel;
      break;
    }
    case Direction::backward: {
      m_pos -= m_orient.front() * vel;
      break;
    }
    case Direction::left: {
      m_pos -= m_orient.right() * vel;
      break;
    }
    case Direction::right: {
      m_pos += m_orient.right() * vel;
      break;
    }
    case Direction::up: {
      m_pos += m_orient.up() * vel;
      break;
    }
    case Direction::down: {
      m_pos -= m_orient.up() * vel;
      break;
    }
  }
  m_updateView = true;
}

auto Camera::rotate(glm::vec2 offset) -> void
{
  offset *= settings.sensitivity;

  m_orient.rotate(offset);
  m_updateView = true;
}

auto Camera::zoom(float offset) -> void
{
  settings.fov -= offset;
  if(settings.fov < 1.0f){
    settings.fov = 1.0f;
  }
  else if(settings.fov > 45.0f) {
    settings.fov = 45.0f;
  }
  m_updateProjection = true;
}

auto Camera::raycastDirection(glm::vec2 glViewPoint) const -> glm::vec3
{
  const auto half_h = settings.near * std::tan(glm::radians(0.5f * settings.fov));
  const auto half_w = m_whRatio * half_h;
  const auto view_click = glm::vec2{
    glViewPoint.x * half_w,
    glViewPoint.y * half_h
  };

  const auto forward = m_orient.front() * settings.near;
  const auto right = m_orient.right() * view_click.x;
  const auto up = m_orient.up() * view_click.y;
  return glm::normalize(forward + right + up);
}
