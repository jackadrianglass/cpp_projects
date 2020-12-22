//
// Created by Jack Glass on 2020-09-29.
//

#ifndef MYPROJECT_APPLICATION_H
#define MYPROJECT_APPLICATION_H

#include <GLFW/glfw3.h>
#include <memory>
#include <string>

class CallbackInterface {
public:
  virtual auto keyCallback(int key, int scancode, int action, int mods) -> void = 0;
  virtual auto mouseButtonCallback(int button, int action, int mods) -> void = 0;
  virtual auto cursorPosCallback(double xpos, double ypos) -> void = 0;
  virtual auto scrollCallback(double xoffset, double yoffset) -> void = 0;
};

struct WindowDeleter {
  void operator() (GLFWwindow* window) const {
    glfwDestroyWindow(window);
  }
};

class Application
{
protected:
  std::unique_ptr<GLFWwindow, WindowDeleter> m_window;
  std::shared_ptr<CallbackInterface> m_callbacks;
public:
  Application(int width, int height, const std::string& name, std::shared_ptr<CallbackInterface> callbacks);
  virtual ~Application();

  auto run() -> void;
  virtual auto draw() -> void = 0;
  virtual auto imgui_draw() -> void = 0;
  auto getWindow() {return m_window.get();}

private:
  static auto defaultWindowSizeCallback(GLFWwindow* window, int width, int height) -> void;
  static auto error_callback(int error, const char* description) -> void;
  static void keyMetaCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void mouseButtonMetaCallback(GLFWwindow *window, int button, int action, int mods);
  static void cursorPosMetaCallback(GLFWwindow *window, double xpos, double ypos);
  static void scrollMetaCallback(GLFWwindow *window, double xoffset, double yoffset);
};

#endif//MYPROJECT_APPLICATION_H
