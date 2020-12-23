//
// Created by Jack Glass on 2020-11-27.
//

#pragma once

#include <optional>
#include "shader_program.h"
#include "sphere.h"
#include "surface.h"
#include "texture.h"
#include "transform.h"

class Planetoid
{
private:
  Sphere m_sphere;
  gl::Surface m_glSphere;
  gl::Texture m_texture;

  float m_theta;        // where in the orbit are we
  const float m_radius; // orbital radius
  const float m_phi;    // orbit plane angle
  const bool m_rotate;
  Transform m_trans;

public:
  Planetoid(
    std::string texture_path,
    glm::vec2 orbit,
    float tilt = 0.0f,
    float scale = 1.0f,
    bool rotate = false
    );

  auto update(float offset) -> void;
  auto draw(const gl::ShaderProgram& shader, std::optional<glm::mat4> transform = std::nullopt) -> void;

  auto transform() const -> const Transform& { return m_trans; }
};
