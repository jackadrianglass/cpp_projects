//
// Created by Jack Glass on 2020-11-14.
//

#include <imgui.h>
#include <renderer.h>
#include "surface_scene.h"
SurfaceScene::SurfaceScene(const mdl::Camera &cam, const glm::vec2 &dim, const glm::vec2 &cursor, const glm::vec2 &glfwCursor)
  : m_camera(cam)
  , m_screenDimensions(dim)
  , m_cursorPos(cursor)
  , m_glfwCursorPos(glfwCursor)
  , m_controlsWindow()
  , m_currentIdx{0}
  , m_showGrid{true}
  , m_glXyGrid()
  , m_grids{
      mdl::Grid({0, 2}, 1, mdl::Plane::XZ, mdl::Algorithm::bspline, 1),
      mdl::Grid({0, 1}, 1, mdl::Plane::XZ, mdl::Algorithm::bezier, 2),
      mdl::Grid({0, 2}, 1, mdl::Plane::XZ, mdl::Algorithm::bspline, 3)
    }
  , m_glGrids{
      std::make_unique<gl::Surface>(),
      std::make_unique<gl::Surface>(),
      std::make_unique<gl::Surface>()
    }
  , m_controlPoints()
{
  for(auto i = 0; i < m_grids.size(); i++){
    m_glGrids[i]->setVertices(m_grids[i].vertices());
    m_glGrids[i]->setIndices(m_grids[i].indices());
  }
  auto xyGrid = mdl::GridOutline({-10, 10}, 0.5f, 0.0f, mdl::Plane::XY);
  m_glXyGrid.setPoints(xyGrid.points());
  m_controlPoints.setPoints(m_grids[0].points());
}

auto SurfaceScene::keyCallback(int, int, int, int) -> void
{
}

auto SurfaceScene::mouseButtonCallback(int, int, int) -> void
{
}

auto SurfaceScene::scrollCallback(double, double) -> void
{
}

auto SurfaceScene::update() -> void
{
}

auto SurfaceScene::draw(gl::ShaderProgram &shader) -> void
{
  if(m_showGrid){
    gl::drawLines(shader, m_glXyGrid);
  }

  gl::draw(shader, *m_glGrids[m_currentIdx]);
  gl::drawPoints(shader, *m_glGrids[m_currentIdx]);
  gl::drawPoints(shader, m_controlPoints);
}

auto SurfaceScene::imgui_draw() -> void
{
  ImGui::Begin("Surface Scene Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("First and last surfaces are bspline, 2nd is bezier");
  ImGui::Text("Surface Idx");
  ImGui::SliderInt("", &m_currentIdx, 0, static_cast<int>(m_grids.size()) - 1);
  if(ImGui::Button(m_showGrid ? "Hide GridOutline" : "Show GridOutline")){
    m_showGrid = !m_showGrid;
  }
  auto pos = ImGui::GetWindowPos();
  auto size = ImGui::GetWindowSize();
  m_controlsWindow = {{pos.x, pos.y}, {size.x, size.y}};
  ImGui::End();
}

auto SurfaceScene::blockCameraMovements() -> bool
{
  return glfw::intersects(m_controlsWindow, m_glfwCursorPos);
}

