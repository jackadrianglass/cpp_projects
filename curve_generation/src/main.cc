//
// Created by jacka on 2020-11-10.
//

#include "callbacks.h"
#include "window.h"

auto main() -> int
{
  glfwInit();
  {
    auto d = glm::ivec2{1200, 800};
    auto game = glfw::Window(d.x, d.y, "curves");
    game.setCallbacks(std::make_shared<Callbacks>(d.x, d.y));
    glPointSize(10.0f);
    game.run();
  }
  glfwTerminate();
}