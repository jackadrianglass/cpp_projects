#pragma once
#include "callback_interface.h"
#include "image_buffer.h"
#include "ray_trace.h"
#include "scene.h"

class Callbacks : public glfw::CallbackInterface
{
private:
    glm::ivec2 m_screen;
    glm::vec3 m_viewPoint;

    ImageBuffer m_outImage;
    Scene m_scene;

public:
  explicit Callbacks(glm::vec2 dimensions);
  //glm callbacks
  auto keyCallback(int key, int scancode, int action, int mods) -> void override;
  auto mouseButtonCallback(int button, int action, int mods) -> void override;
  auto cursorPosCallback(glm::vec2 pos) -> void override;
  auto scrollCallback(glm::vec2 size) -> void override;
  auto windowSizeCallback(glm::ivec2 size) -> void override;

  //Window callbacks
  auto update() -> void override;
  auto draw() -> void override;
  auto imgui_draw() -> void override;
};
