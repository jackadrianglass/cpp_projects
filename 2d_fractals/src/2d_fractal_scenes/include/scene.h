//
// Created by Jack Glass on 2020-09-28.
//

#ifndef MYPROJECT_SCENE_H
#define MYPROJECT_SCENE_H

namespace fractals {
  class Scene
  {
  public:
    virtual ~Scene() = default;
    virtual auto draw() const -> void = 0;
    virtual auto increment_depth() -> void = 0;
    virtual auto decrement_depth() -> void = 0;

    static constexpr auto max_depth = 17;
  };
}

#endif//MYPROJECT_SCENE_H
