//
// Created by Jack Glass on 2020-11-27.
//

#include "planetoid.h"
#include "renderer.h"
#include <optional>

Planetoid::Planetoid(std::string texture_path, glm::vec2 orbit, float tilt, float scale, bool rotate)
  : m_sphere()
  , m_glSphere()
  , m_texture(std::move(texture_path), GL_NEAREST)
  , m_theta{0}
  , m_radius{std::sqrt(orbit.x * orbit.x + orbit.y * orbit.y)}
  , m_phi{std::atan(orbit.y / orbit.x)}
  , m_rotate{rotate}
  , m_trans({orbit, 0}, {tilt, 180, 0}, glm::vec3(scale))
{
  m_glSphere.setVertices(m_sphere.vertices());
  m_glSphere.setIndices(m_sphere.faceList());
}

auto Planetoid::update(float offset) -> void
{
  m_theta += offset;

  using std::cos, std::sin;
  const auto pos = glm::vec3{
    m_radius * cos(m_theta) * cos(m_phi),
    m_radius * cos(m_theta) * sin(m_phi),
    m_radius * sin(m_theta)
  };
  m_trans.moveTo(pos);

  if(m_rotate){
    m_trans.rotate({0, 300 * offset, 0});
  }
}

auto Planetoid::draw(const gl::ShaderProgram &shader, std::optional<glm::mat4> transform) -> void
{
  if(transform){
    shader.uploadUniform("model", *transform * m_trans.matrix());
  } else {
    shader.uploadUniform("model", m_trans.matrix());
  }
  m_texture.bind();
  gl::draw(shader, m_glSphere);
}