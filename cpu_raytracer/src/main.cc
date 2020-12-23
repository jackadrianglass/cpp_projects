//
// Created by Jack Glass on 2020-11-25.
//
#include "callbacks.h"
#include "window.h"

int main() {
  glfwInit();
  {
    auto d = glm::ivec2{1200, 800};
    auto app = glfw::Window(d.x, d.y, "raytracing");
    app.setCallbacks(std::make_shared<Callbacks>(d));
    app.run();
  }
  glfwTerminate();
}