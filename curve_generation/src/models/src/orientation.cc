//
// Created by Jack Glass on 2020-11-13.
//

#include "orientation.h"

constexpr auto max_pitch = 89.0f;
constexpr auto min_pitch = -89.0f;

Orientation::Orientation(float roll, float pitch, float yaw)
  : m_roll{roll}
  , m_pitch{pitch}
  , m_yaw{yaw}
{
}

auto Orientation::rotate(glm::vec2 offset, bool constrainPitch) -> void
{
  m_yaw += offset.x;
  m_pitch += offset.y;

  if(constrainPitch) {
    if(m_pitch > max_pitch){
      m_pitch = max_pitch;
    }
    else if(m_pitch < min_pitch){
      m_pitch = min_pitch;
    }
  }
  m_updateRight = m_updateUp = m_updateFront = true;
}

auto Orientation::front() const -> glm::vec3
{
  if(m_updateFront) {
    m_front = glm::normalize(glm::vec3{
      std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
      std::sin(glm::radians(m_pitch)),
      std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)) });
    m_updateFront = false;
  }
  return m_front;
}

auto Orientation::up() const -> glm::vec3
{
  if(m_updateUp) {
    m_up = glm::cross(right(), front());
    m_updateUp = false;
  }
  return m_up;
}
auto Orientation::right() const -> glm::vec3
{
  if(m_updateRight){
//    m_right = glm::normalize(glm::vec3{
//      std::cos(glm::radians(m_pitch)) * std::cos(glm::radians(m_roll)),
//      std::sin(glm::radians(m_roll)),
//      std::sin(glm::radians(m_pitch)) * std::cos(glm::radians(m_roll))
//    });
    m_right = glm::normalize(glm::cross(front(), {0, 1, 0}));
    m_updateRight = false;
  }
  return m_right;
}
