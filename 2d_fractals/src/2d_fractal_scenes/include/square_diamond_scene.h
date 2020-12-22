//
// Created by Jack Glass on 2020-09-28.
//

#ifndef MYPROJECT_SQUARE_DIAMOND_SCENE_H
#define MYPROJECT_SQUARE_DIAMOND_SCENE_H

#include <vector>
#include <array>
#include "gpu_geometry.h"
#include "square.h"
#include "scene.h"

namespace fractals{
  class SquareDiamondScene : public Scene
  {
  private:
    std::vector<gl::shapes::Square> m_squares;
    size_t m_depth;

    std::array<gl::GpuGeometry, (max_depth + 1)*2> m_geom;

  public:
    SquareDiamondScene();

    auto draw() const -> void override;
    auto increment_depth() -> void override;
    auto decrement_depth() -> void override;
  };
}


#endif//MYPROJECT_SQUARE_DIAMOND_SCENE_H
