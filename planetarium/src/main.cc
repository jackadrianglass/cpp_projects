//
// Created by Jack Glass on 2020-11-25.
//
#include "callbacks.h"
#include "window.h"

int main() {
  glfwInit();
  {
    auto d = glm::ivec2{1200, 800};
    auto game = glfw::Window(d.x, d.y, "planetarium");
    game.setCallbacks(std::make_shared<Callbacks>(d));
    glPointSize(10.0f);
    game.run();
  }
  glfwTerminate();
}