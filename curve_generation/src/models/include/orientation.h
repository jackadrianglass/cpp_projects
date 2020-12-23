//
// Created by Jack Glass on 2020-11-13.
//

#pragma once
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

class Orientation
{
private:
  float m_pitch;
  float m_yaw;
  float m_roll;

  // cache
  mutable bool m_updateFront {true};
  mutable bool m_updateUp {true};
  mutable bool m_updateRight {true};
  mutable glm::vec3 m_front{};
  mutable glm::vec3 m_up{};
  mutable glm::vec3 m_right{};

public:
  Orientation(float roll, float pitch, float yaw);

  auto rotate(glm::vec2 offset, bool constrainPitch = true) -> void;

  auto front() const -> glm::vec3;
  auto up() const -> glm::vec3;
  auto right() const -> glm::vec3;
};