//
// Created by Jack Glass on 2020-11-12.
// Adapted from https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

#pragma once
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "camera_orientation.h"

namespace mdl {
  enum class Direction {
    forward,
    backward,
    left,
    right,
    up,
    down
  };

  struct CameraSettings {
    float sensitivity;  // movement based sensitivity
    float fov;          // field of view in degrees
    float speed;
    float near;         // distance of the near plane from the camera
    float far;          // distance of the far plane from the camera
    bool  constrain_pitch;
  };

  constexpr auto defaultSettings = CameraSettings{
    15.0f,
    45.0f,
    5.0f,
    0.1f,
    200.0f,
    true
  };

  class Camera
  {
  public:
    CameraSettings settings;

  private:
    glm::vec3 m_pos;
    CameraOrientation m_orient;

    // cache
    mutable bool m_updateView{true};
    mutable bool m_updateProjection{true};

    mutable glm::mat4 m_view{};
    mutable glm::mat4 m_projection{};
    mutable float m_whRatio{0};          //width to height ration

  public:
    Camera(glm::vec3 pos, CameraOrientation orientation, CameraSettings settings = defaultSettings);

    // setters
    auto move(Direction direction, double deltaTime) -> void;
    auto rotate(glm::vec2 offset) -> void;
    auto zoom(float offset) -> void;

    //getters
    [[nodiscard]]
    auto viewMatrix() const -> glm::mat4;
    [[nodiscard]]
    auto projectionMatrix(glm::vec2 windowDimensions) const -> glm::mat4;
    [[nodiscard]]
    auto position() const { return m_pos; }

    [[nodiscard]]
    auto raycastDirection(glm::vec2 glViewPoint) const -> glm::vec3;
  };
}