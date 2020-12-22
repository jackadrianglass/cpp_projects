#include <utility>
#include <vector>
#include <memory>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "shader_program.h"
#include "application.h"

#include "dragon_curve_scene.h"
#include "koch_snowflake_scene.h"
#include "square_diamond_scene.h"
#include "serpinsky_scene.h"
#include "hilbert_scene.h"

using namespace fractals;

class Assignment1 : public Application {
private:
  gl::ShaderProgram m_shader;
  std::vector<std::unique_ptr<Scene>> m_scenes;
  int m_idx;

public:
  explicit Assignment1(std::shared_ptr<CallbackInterface> callbacks)
  : Application(800, 600, "Assignment 1", std::move(callbacks))
  , m_shader("shaders/test.vert", "shaders/test.frag")
  , m_scenes()
  , m_idx(0)
  {
    m_scenes.push_back(std::make_unique<HilbertScene>());
    m_scenes.push_back(std::make_unique<SerpinskyScene>());
    m_scenes.push_back(std::make_unique<KochSnowflakeScene>());
    m_scenes.push_back(std::make_unique<SquareDiamondScene>());
    m_scenes.push_back(std::make_unique<DragonCurveScene>());
  }

  ~Assignment1() override {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  auto increment_depth() -> void {
    m_scenes[m_idx]->increment_depth();
  }

  auto decrement_depth() -> void {
    m_scenes[m_idx]->decrement_depth();
  }
  auto next_scene() -> void {
    m_idx++;
    if(m_idx >= m_scenes.size()){
      m_idx = 0;
    }
  }
  auto prev_scene() -> void {
    m_idx--;
    if(m_idx >= m_scenes.size()){
      m_idx = static_cast<int>(m_scenes.size()) - 1;
    }
  }

  auto draw() -> void override {
    m_shader.use();
    m_scenes[m_idx]->draw();
  }

  auto imgui_draw() -> void override {
    ImGui::Begin("Controls");
    if(ImGui::Button("Increment Depth")){
      increment_depth();
    }
    if(ImGui::Button("Decrement Depth")){
      decrement_depth();
    }
    if(ImGui::Button("Previous Scene")){
      prev_scene();
    }
    if(ImGui::Button("Next Scene")){
      next_scene();
    }
    ImGui::End();
  };
};

class A1Callbacks : public CallbackInterface {
private:
  Assignment1 * m_parent;

public:
  explicit A1Callbacks()
    : m_parent(nullptr)
  {}

  auto setParent(Assignment1* parent){
    m_parent = parent;
  }

  auto keyCallback(int key, int scancode, int action, int mods) -> void override
  {
    if(m_parent == nullptr) return;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
      glfwSetWindowShouldClose(m_parent->getWindow(), true);
    }
    if(action == GLFW_PRESS){
      switch (key) {
      case GLFW_KEY_UP:
      {
        m_parent->increment_depth();
        break;
      }
      case GLFW_KEY_DOWN:
      {
        m_parent->decrement_depth();
        break;
      }
      case GLFW_KEY_LEFT:
      {
        m_parent->prev_scene();
        break;
      }
      case GLFW_KEY_RIGHT:
      {
        m_parent->next_scene();
        break;
      }
      default:
        break;
      }
    }
  }
  auto mouseButtonCallback(int, int, int) -> void override {}
  auto cursorPosCallback(double, double) -> void override {}
  auto scrollCallback(double, double) -> void override{}
};

auto main() -> int
{
  glfwInit();
  {
    auto callbacks = std::make_shared<A1Callbacks>();
    auto app = Assignment1(callbacks);
    callbacks->setParent(&app);
    app.run();
  }
  glfwTerminate();
}
