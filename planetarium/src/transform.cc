//
// Created by Jack Glass on 2020-11-28.
//

#include "transform.h"
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
  : m_pos{pos}
  , m_rotation{rot}
  , m_scale{scale}
  , m_update{true}
  , m_transform()
{
}

auto Transform::pos() const -> glm::vec3
{
  return m_pos;
}

auto Transform::rotation() const -> glm::vec3
{
  return m_rotation;
}

auto Transform::scale() const -> glm::vec3
{
  return m_scale;
}

auto Transform::matrix() const -> glm::mat4
{
  if(m_update){
    m_transform = glm::mat4(1.0f); // identity mat
    m_transform = glm::translate(m_transform, m_pos);
    m_transform = glm::rotate(m_transform, glm::radians(m_rotation.x), {1, 0, 0});
    m_transform = glm::rotate(m_transform, glm::radians(m_rotation.y), {0, 1, 0});
    m_transform = glm::rotate(m_transform, glm::radians(m_rotation.z), {0, 0, 1});
    m_transform = glm::scale(m_transform, m_scale);

    m_update = false;
  }
  return m_transform;
}

auto Transform::move(glm::vec3 offset) -> void
{
  m_pos += offset;
  m_update = true;
}

auto Transform::moveTo(glm::vec3 position) -> void
{
  m_pos = position;
  m_update = true;
}

auto Transform::rotate(glm::vec3 offset) -> void
{
  m_rotation += offset;
  m_update = true;
}
