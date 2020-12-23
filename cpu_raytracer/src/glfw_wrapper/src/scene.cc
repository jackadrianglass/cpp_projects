//
// Created by Jack Glass on 2020-11-14.
//

#include "scene.h"

auto glfw::intersects(const glfw::WindowData &window, glm::vec2 pos) -> bool
{
  auto x = (pos.x >= window.pos.x) && (pos.x <= window.pos.x + window.size.x);
  auto y = (pos.y >= window.pos.y) && (pos.y <= window.pos.y + window.size.y);
  return x && y;
}
