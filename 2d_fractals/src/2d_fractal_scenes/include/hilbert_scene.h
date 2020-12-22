//
// Created by Jack Glass on 2020-09-30.
//

#ifndef MYPROJECT_HILBERT_SCENE_H
#define MYPROJECT_HILBERT_SCENE_H

#include <scene.h>
#include "gpu_geometry.h"

namespace fractals
{
  class HilbertScene : public Scene
  {
  private:
    int m_depth;
    gl::GpuGeometry m_geom;
    size_t m_num_vertices;

    auto recalculate_geometry() -> void;

  public:
    HilbertScene();
    auto draw() const -> void override;
    auto increment_depth() -> void override;
    auto decrement_depth() -> void override;
  };
}



#endif//MYPROJECT_HILBERT_SCENE_H
