//
// Created by Jack Glass on 2020-09-28.
//

#ifndef MYPROJECT_SERPINSKY_SCENE_H
#define MYPROJECT_SERPINSKY_SCENE_H

#include "scene.h"
#include "gpu_geometry.h"

namespace fractals{
  class SerpinskyScene : public Scene
  {
  public:
    SerpinskyScene();
    auto draw() const -> void override;
    auto increment_depth() -> void override;
    auto decrement_depth() -> void override;

  private:
    auto recalculate_geometry() -> void;
    gl::GpuGeometry m_current_geometry;
    int m_current_depth;
    size_t m_number_of_vertices;
  };
}

#endif//MYPROJECT_SERPINSKY_SCENE_H
