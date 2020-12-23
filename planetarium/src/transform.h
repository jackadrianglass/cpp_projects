//
// Created by Jack Glass on 2020-11-28.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform
{
private:
  glm::vec3 m_pos;
  glm::vec3 m_rotation; //x is pitch, y is yaw, z is roll
  glm::vec3 m_scale;

  mutable bool m_update;
  mutable glm::mat4 m_transform;

public:
  Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale = {1.0f, 1.0f, 1.0f});

  // getters
  auto pos() const -> glm::vec3;
  auto rotation() const -> glm::vec3;
  auto scale() const -> glm::vec3;
  auto matrix() const -> glm::mat4;

  // setters
  auto move(glm::vec3 offset) -> void;
  auto moveTo(glm::vec3 position) -> void;
  auto rotate(glm::vec3 offset) -> void;
};
