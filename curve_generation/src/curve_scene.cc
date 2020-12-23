//
// Created by Jack Glass on 2020-11-14.
//

#include <imgui.h>
#include "curve_scene.h"
#include "renderer.h"

#include <GLFW/glfw3.h>

CurveScene::CurveScene(const mdl::Camera& cam, const glm::vec2& dim, const glm::vec2& cursor, const glm::vec2& glfw_cursor)
  : m_camera(cam)
  , m_screenDimensions(dim)
  , m_cursorPos(cursor)
  , m_glfwCursorPos(glfw_cursor)
  , m_curve(4)
  , m_grid({-10, 10}, 0.5f, 0.0f, mdl::Plane::XY)
  , m_surface(m_curve)
  , m_isPressed{false}
  , m_showGrid{true}
  , m_drawSurface{false}
{
  m_glGrid.setPoints(m_grid.points());
}

auto CurveScene::keyCallback(int key, int, int action, int) -> void
{
  if(action == GLFW_PRESS && (key == GLFW_KEY_DELETE || GLFW_KEY_BACKSPACE)){
    m_curve.deleteSelected();
  }
}

auto CurveScene::mouseButtonCallback(int button, int action, int mods) -> void
{
  glm::vec3 xy_pos{};
  if(action == GLFW_PRESS){
    // cast a ray to intersect with the xy plane
    const auto dir = m_camera.raycastDirection(m_cursorPos);
    const auto times = -m_camera.position().z / dir.z;
    xy_pos = m_camera.position() + times * dir;
  }

  if(button == GLFW_MOUSE_BUTTON_LEFT){
    switch(action){
    case GLFW_PRESS: {
      m_curve.selectPointCloseTo(xy_pos);
      m_isPressed = true;
      break;
    }
    case GLFW_RELEASE: {
      m_isPressed = false;
      break;
    }
    default:{
      //do nothing
    }
    }
  }

  if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    m_curve.addCtrlPoint(xy_pos);
  }
}

auto CurveScene::scrollCallback(double xoffset, double yoffset) -> void
{
}

auto CurveScene::update() -> void
{
  if(m_isPressed && m_curve.havePointSelected()){
    const auto dir = m_camera.raycastDirection(m_cursorPos);
    const auto times = -m_camera.position().z / dir.z;
    const auto pos = m_camera.position() + times * dir;

    m_curve.moveSelected(pos);
  }
}

auto CurveScene::draw(gl::ShaderProgram &shader) -> void
{
  // GridOutline
  if(m_showGrid){
    gl::drawLines(shader, m_glGrid);
  }

  // Control Points
  if(!m_drawSurface){
    if(!m_curve.ctrlPoints().empty()){
      // control points
      if(!m_curve.isUpdated()){
        m_glPoints.setPoints(m_curve.ctrlPoints());
      }
      if(!m_curve.ctrlPoints().empty()){
        gl::drawPoints(shader, m_glPoints);
        gl::draw(shader, m_glPoints);
      }

      // Basic Curve
      if(!m_curve.isUpdated()){
        m_glCurve.setPoints(m_curve.curve());
      }
      if(!m_curve.curve().empty()){
        gl::draw(shader, m_glCurve);
      }
    }
  }
  else {
    if(!m_surface.isUpdated()){
      const auto vert = m_surface.surfaceVertices();
      const auto idx = m_surface.surfaceIndices();
      m_glSurface.setVertices(vert);
      m_glSurface.setIndices(idx);
    }
    gl::draw(shader, m_glSurface);
  }
}

auto CurveScene::imgui_draw() -> void
{
  ImGui::Begin("Curve Scene Information", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Curve Type %s", m_curve.curveType() == mdl::Algorithm::bezier ? "Bezier" : "B-Spline");

  const auto& selected = m_curve.getSelected();
  ImGui::Text("Selected Point\n\tpos [%f, %f]", selected.pos.x, selected.pos.y);

  ImGui::BeginChild("Control Points", ImVec2(0, ImGui::GetFontSize() * 8.0f), true, ImGuiWindowFlags_MenuBar);
  if (ImGui::BeginMenuBar())
  {
    ImGui::TextUnformatted("Control Points");
    ImGui::EndMenuBar();
  }
  auto idx = 0;
  for(const auto & point : m_curve.ctrlPoints()){
    ImGui::Text("Point %d\n\tpos [%f, %f]", idx, point.pos.x, point.pos.y);
    idx++;
  }
  ImGui::EndChild();

  // update window stats
  auto pos = ImGui::GetWindowPos();
  auto size = ImGui::GetWindowSize();
  m_infoWindow = {{pos.x, pos.y}, {size.x, size.y}};
  ImGui::End();

  ImGui::Begin("Curve Scene Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Use delete or backspace to delete selected");
  if(ImGui::Button("Toggle curve type ")) {
    toggleCurveType();
  }
  ImGui::Text("Number of Subdivisions");
  auto num = m_curve.subdivisions();
  int max = 50;
  if(m_curve.curveType() == mdl::Algorithm::bspline){
    max = 5;
  }
  ImGui::SliderInt("", &num, 1, max);
  m_curve.setNumSubdivisions(num);
  if(ImGui::Button("Clear Control Points")) {
    m_curve.clear();
  }
  if(ImGui::Button(m_showGrid ? "Hide GridOutline" : "Show GridOutline")){
    m_showGrid = !m_showGrid;
  }
  if(ImGui::Button(m_drawSurface ? "Hide Volume" : "Show Volume")){
    m_drawSurface = !m_drawSurface;
    if(m_drawSurface) m_surface.setUpdateFlag();
  }
  pos = ImGui::GetWindowPos();
  size = ImGui::GetWindowSize();
  m_controlsWindow = {{pos.x, pos.y}, {size.x, size.y}};
  ImGui::End();
}

auto CurveScene::toggleCurveType() -> void
{
  using namespace mdl;
  switch(m_curve.curveType()){
  case Algorithm::bezier: {
    m_curve.setCurveType(Algorithm::bspline);
    break;
  }
  case Algorithm::bspline: {
    m_curve.setCurveType(Algorithm::bezier);
    break;
  }
  }
}

auto CurveScene::blockCameraMovements() -> bool
{
  auto info = glfw::intersects(m_infoWindow, m_glfwCursorPos);
  auto controls = glfw::intersects(m_controlsWindow, m_glfwCursorPos);
  return m_curve.havePointSelected() || info || controls;
}
