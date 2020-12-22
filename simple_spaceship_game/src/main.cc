#include <utility>
#include <vector>
#include <memory>
#include <ctime>
#include <imgui.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <spdlog/spdlog.h>

#include "gpu_geometry.h"
#include "shader_program.h"
#include "texture.h"
#include "window.h"

struct CPU_Geometry {
  std::vector<glm::vec3> verts;
  std::vector<glm::vec2> texCoords;
};

CPU_Geometry shipGeom(){
  CPU_Geometry retGeom;
  // vertices
  retGeom.verts.emplace_back(-1.f, 1.f, 0.f);
  retGeom.verts.emplace_back(-1.f, -1.f, 0.f);
  retGeom.verts.emplace_back(1.f, -1.f, 0.f);
  retGeom.verts.emplace_back(-1.f, 1.f, 0.f);
  retGeom.verts.emplace_back(1.f, -1.f, 0.f);
  retGeom.verts.emplace_back(1.f, 1.f, 0.f);

  // texture coordinates
  retGeom.texCoords.emplace_back(0.f, 1.f);
  retGeom.texCoords.emplace_back(0.f, 0.f);
  retGeom.texCoords.emplace_back(1.f, 0.f);
  retGeom.texCoords.emplace_back(0.f, 1.f);
  retGeom.texCoords.emplace_back(1.f, 0.f);
  retGeom.texCoords.emplace_back(1.f, 1.f);
  return retGeom;
}

[[nodiscard]]
auto angle_between(glm::vec2 first, glm::vec2 second) -> float{
  return std::acos(glm::dot(first, second) / (glm::length(first) * glm::length(second)));
}

[[nodiscard]]
auto direction(glm::vec2 first, glm::vec2 second) -> float{
  if(first.x * second.y - first.y * second.x > 0){
    return -1.0f;
  }
  return 1.0f;
}

enum class ObjectType {
  ship,
  diamond,
  flame
};

struct GameObject {
  const ObjectType m_type;
  const CPU_Geometry cgeom;
  gl::GpuGeometry ggeom;
  gl::Texture texture;

  mutable glm::mat4 m_transformation;
  mutable bool m_transValid;
  glm::vec3 m_pos;
  glm::vec4 m_dir;
  float m_theta; //in radians
  float m_scale;
  float m_collisionRadius;

  GameObject(std::string texturePath, ObjectType type, glm::vec3 starting_pos, float scale)
    : texture(std::move(texturePath), GL_NEAREST)
    , cgeom(shipGeom())
    , m_pos(starting_pos)
    , m_dir{0.0f, 1.0f, 0.0f, 1.0f}
    , m_scale(scale)
    , m_type(type)
    , m_collisionRadius(0.8f * scale)
    , m_theta{0.0f}
    , m_transValid{false}
    , m_transformation(1.0f)
  {
    ggeom.setVertices(cgeom.verts);
    ggeom.setTexCoord(cgeom.texCoords);
  }
  auto reset(glm::vec3 position, float scale) {
    m_transValid = false;
    m_pos = position;
    m_scale = scale;
    m_theta = 0.0f;
    m_dir = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f};
    m_collisionRadius = 0.8f * scale;
  }

  auto rotate(float theta) -> void {
    m_transValid = false;
    m_theta += theta;
    m_dir = glm::rotate(theta, glm::vec3{0.0f, 0.0f, 1.0f}) * m_dir;
  }

  auto moveForward(float amount) -> void {
    m_transValid = false;
    m_pos += amount * glm::vec3{m_dir.x, m_dir.y, m_dir.z};
  }

  auto scale(float factor) -> void {
    m_transValid = false;
    m_scale *= factor;
    m_collisionRadius *= factor;
  }

  [[nodiscard]]
  auto hasCollided(const GameObject& other) const -> bool {
    return glm::length(other.m_pos - m_pos) < other.m_collisionRadius + m_collisionRadius;
  }

  auto uploadTransformation(const gl::ShaderProgram& shader) const {
    static const std::string uniform_name = "u_trans";

    if(!m_transValid) {
      m_transformation= glm::mat4(1.0f);
      m_transformation = glm::translate(m_transformation, m_pos);
      m_transformation = glm::scale(m_transformation, glm::vec3(m_scale));
      m_transformation = glm::rotate(m_transformation, m_theta, glm::vec3{0.0f, 0.0f, 1.0f});
      m_transValid = true;
    }

    shader.uploadUniform(uniform_name, m_transformation);
  }

  auto draw(const gl::ShaderProgram& shader) const -> void {
    uploadTransformation(shader);
    ggeom.bind();
    texture.bind();
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(cgeom.verts.size()));
  }
};

struct Step {
  int num_steps;
  int max_steps;
  float step_size;

  [[nodiscard]]
  auto isSteppingDone() const -> bool {return num_steps >= max_steps;}
};

enum class GameState {
  running,
  won,
  lost,
};

class shipGame : public gcn::CallbackInterface {
public:
  gl::ShaderProgram m_shader;
  GameObject m_ship;
  std::vector<std::unique_ptr<GameObject>> m_diamonds;
  Step m_shipRotation;
  GameState m_state;

  constexpr static auto rotation_step = glm::radians(4.0f);
  constexpr static auto translation_step = 0.01f;
  constexpr static int max_num_diamonds = 6;

  float m_rotationDir;
  float m_translationDir;
  int m_score;

  glm::vec2 m_cursorPos;
  glm::vec2 m_screenDimensions;

  auto reset(){
    m_score = 0;
    m_ship.reset({0.0f, 0.0f, 0.0f}, 0.05f);
    m_diamonds.clear();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for(auto i = 0; i < max_num_diamonds; i++){
      auto randCoor = [](){return (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 1.5f - 0.75f;};
      m_diamonds.emplace_back(std::make_unique<GameObject>("textures/diamond.png", ObjectType::diamond, glm::vec3{randCoor(), randCoor(), 0}, 0.05f));
    }
  }

  shipGame(double width, double height)
    : CallbackInterface()
    , m_ship("textures/ship.png", ObjectType::ship, {0.0f, 0.0f, 0.0f}, 0.05f)
    , m_shader("shaders/test.vert", "shaders/test.frag")
    , m_diamonds()
    , m_shipRotation{0, 0, 0.0f}
    , m_rotationDir{0}
    , m_translationDir{0}
    , m_score{0}
    , m_cursorPos{0, 0}
    , m_screenDimensions{width, height}
    , m_state(GameState::running)
  {
    reset();
  }


  //glm callbacks
  auto keyCallback(int key, [[maybe_unused]]int scancode, int action, [[maybe_unused]]int mods) -> void override {
    switch(key){
      case GLFW_KEY_W: [[fallthrough]];
      case GLFW_KEY_UP:
        //Move forward
        if(action == GLFW_PRESS){
          m_translationDir += 1.0f;
        }
        else if(action == GLFW_RELEASE){
          m_translationDir -= 1.0f;
        }
        break;
      case GLFW_KEY_S: [[fallthrough]];
      case GLFW_KEY_DOWN:
        //Move backwards
        if(action == GLFW_PRESS){
          m_translationDir -= 1.0f;
        }
        else if(action == GLFW_RELEASE){
          m_translationDir += 1.0f;
        }
        break;
      case GLFW_KEY_A: [[fallthrough]];
      case GLFW_KEY_LEFT:
        //rotate in positive direction
        if(action == GLFW_PRESS){
          m_rotationDir += 1.0f;
        }
        else if(action == GLFW_RELEASE){
          m_rotationDir -= 1.0f;
        }
        break;
      case GLFW_KEY_D: [[fallthrough]];
      case GLFW_KEY_RIGHT:
        //rotate in negative direction
        if(action == GLFW_PRESS){
          m_rotationDir -= 1.0f;
        }
        else if(action == GLFW_RELEASE){
          m_rotationDir += 1.0f;
        }
        break;
      case GLFW_KEY_R:
        reset();
        break;
      default: break;
    }

  }

  [[nodiscard]]
  auto mouseGl() const -> glm::vec2 {
    auto shiftedVec = m_cursorPos - glm::vec2{0.5f, 0.5f};
    auto scaledToZeroOne = shiftedVec / m_screenDimensions;

    auto flippedY = glm::vec2{scaledToZeroOne.x, 1.0f - scaledToZeroOne.y};

    auto final = flippedY * 2.0f - glm::vec2(1.0f, 1.0f);
    return final;
  }

  auto mouseButtonCallback(int button, int action, [[maybe_unused]]int mods) -> void override {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
      auto position = mouseGl();
      m_shipRotation.max_steps = 20;
      m_shipRotation.num_steps = 0;
      m_shipRotation.step_size = (angle_between(position, m_ship.m_dir) * direction(position, m_ship.m_dir)) / static_cast<float>(m_shipRotation.max_steps);
    }
  }

  auto cursorPosCallback(double x_pos, double y_pos) -> void override {m_cursorPos = {x_pos, y_pos};}
  auto scrollCallback(double, double) -> void override {}

  auto windowSizeCallback(int width, int height) -> void override {
    m_screenDimensions = {width, height};
    glViewport(0, 0, width, height);
  }

  auto update() {
    // Update the ship coordinates
    if(m_rotationDir != 0){
      m_ship.rotate(rotation_step * m_rotationDir);
    }
    if(m_translationDir != 0){
      m_ship.moveForward(translation_step * m_translationDir);
    }
    if(!m_shipRotation.isSteppingDone()){
      m_ship.rotate(m_shipRotation.step_size);
      m_shipRotation.num_steps++;
    }

    // Check collisions
    auto end = std::remove_if(m_diamonds.begin(), m_diamonds.end(),
                              [&](const auto& d){
                                     if(m_ship.hasCollided(*d)){
                                       m_ship.scale(1.15f);
                                       m_score++;
                                       return true;
                                     }
                                     return false;
                              });
    m_diamonds.erase(end, m_diamonds.end());
    if(m_diamonds.empty()){
      m_state = GameState::won;
    }
  }

  auto draw() -> void override {
    if(m_state == GameState::running){
      update();
    }
    // draw the ship
    m_shader.use();
    m_ship.draw(m_shader);

    // draw diamonds
    for(const auto & diamond : m_diamonds){
      diamond->draw(m_shader);
    }
  }

  auto imgui_draw() -> void override {
    if(m_state == GameState::won){
      ImGui::Begin("Game Won");
      if(ImGui::Button("Play Again?")){
        m_state = GameState::running;
        reset();
      }
      ImGui::End();
    }

    if(m_state == GameState::lost){
      ImGui::Begin("Game Lost");
      if(ImGui::Button("Play Again?")){
        m_state = GameState::running;
        reset();
      }
      ImGui::End();
    }

    // Putting the text-containing window in the top-left of the screen.
    ImGui::SetNextWindowPos(ImVec2(5, 5));

    // Setting flags
    ImGuiWindowFlags textWindowFlags =
      ImGuiWindowFlags_NoMove |				// text "window" should not move
      ImGuiWindowFlags_NoResize |				// should not resize
      ImGuiWindowFlags_NoCollapse |			// should not collapse
      ImGuiWindowFlags_NoSavedSettings |		// don't want saved settings mucking things up
      ImGuiWindowFlags_AlwaysAutoResize |		// window should auto-resize to fit the text
      ImGuiWindowFlags_NoBackground |			// window should be transparent; only the text should be visible
      ImGuiWindowFlags_NoDecoration |			// no decoration; only the text should be visible
      ImGuiWindowFlags_NoTitleBar;			// no title; only the text should be visible

    ImGui::Begin("scoreText", nullptr, textWindowFlags);
    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("Score: %d", m_score);

    // End the window.
    ImGui::End();
  }
};


auto main() -> int
{
  glfwInit();
  {
    auto game = gcn::Window(800, 600, "ship game");
    game.setCallbacks(std::make_shared<shipGame>(800, 600));
    game.run();
  }
  glfwTerminate();
}
