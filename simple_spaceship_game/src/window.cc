//
// Created by Jack Glass on 2020-09-29.
//
#include "debug.h"
#include "window.h"

#include <stdexcept>
#include <spdlog/spdlog.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace gcn;

Window::Window(int width, int height, const std::string& name)
: m_window(nullptr)
, m_callbacks(nullptr)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  m_window = std::unique_ptr<GLFWwindow, WindowDeleter>(glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr));
  if(m_window == nullptr){
    throw std::runtime_error("Unable to create a glfw window");
  }

  glfwMakeContextCurrent(m_window.get());

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    throw std::runtime_error("Unable to load opengl functions");
  }

  // set callbacks
  gl::debug::enable();
  glfwSetErrorCallback(error_callback);
  glfwSetKeyCallback(m_window.get(), keyMetaCallback);
  glfwSetMouseButtonCallback(m_window.get(), mouseButtonMetaCallback);
  glfwSetCursorPosCallback(m_window.get(), cursorPosMetaCallback);
  glfwSetScrollCallback(m_window.get(), scrollMetaCallback);
  glfwSetWindowSizeCallback(m_window.get(), defaultWindowSizeCallback);
  glViewport(0, 0, width, height);

  // setup ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(m_window.get(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

Window::~Window()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

auto Window::run() -> void
{
  while (!glfwWindowShouldClose(m_window.get())){
    glfwPollEvents();

    glEnable(GL_FRAMEBUFFER_SRGB);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_callbacks->draw();
    glDisable(GL_FRAMEBUFFER_SRGB);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    m_callbacks->imgui_draw();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window.get());
  }
}

auto Window::error_callback(int error, const char *description) -> void
{
  spdlog::error("GLFW ERROR {}: {}", error, description);
}

auto Window::defaultWindowSizeCallback(GLFWwindow* window, int width, int height) -> void
{
  auto* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));
  if(!callbacks) return;
  callbacks->windowSizeCallback(width, height);
}

void Window::keyMetaCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));
  if(!callbacks) return;
  callbacks->keyCallback(key, scancode, action, mods);
}


void Window::mouseButtonMetaCallback(GLFWwindow* window, int button, int action, int mods) {
  auto* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));
  if(!callbacks) return;
  callbacks->mouseButtonCallback(button, action, mods);
}


void Window::cursorPosMetaCallback(GLFWwindow* window, double xpos, double ypos) {
  auto* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));
  if(!callbacks) return;
  callbacks->cursorPosCallback(xpos, ypos);
}


void Window::scrollMetaCallback(GLFWwindow* window, double xoffset, double yoffset) {
  auto* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));
  if(!callbacks) return;
  callbacks->scrollCallback(xoffset, yoffset);
}