//
// Created by Jack Glass on 2020-09-29.
//

#ifndef MYPROJECT_KOCH_SNOWFLAKE_SCENE_H
#define MYPROJECT_KOCH_SNOWFLAKE_SCENE_H

#include "scene.h"
#include "gpu_geometry.h"

namespace fractals{
  class KochSnowflakeScene : public Scene
  {
  private:
    int m_depth;
    gl::GpuGeometry m_geom;
    size_t m_num_vertices;

    auto recalculate_geometry() -> void;
  public:
    KochSnowflakeScene();
    auto draw() const -> void override;
    auto increment_depth() -> void override;
    auto decrement_depth() -> void override;
  };
}

#endif//MYPROJECT_KOCH_SNOWFLAKE_SCENE_H
