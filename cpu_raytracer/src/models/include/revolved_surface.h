//
// Created by Jack Glass on 2020-11-14.
//

#pragma once
#include <vector>
#include "point.h"
#include "curve.h"

namespace mdl{
    class RevolvedSurface
    {
      private:
        const Curve* m_curve;

        bool m_updated;
        std::vector<gl::Point> m_vertices;
        std::vector<unsigned int> m_indices;

      public:
        explicit RevolvedSurface(const Curve& curve);

        auto setUpdateFlag() -> void;

        [[nodiscard]]
        auto isUpdated() -> bool;
        [[nodiscard]]
        auto surfaceVertices() -> std::vector<gl::Point>;
        [[nodiscard]]
        auto surfaceIndices() -> std::vector<unsigned int>;
    };
}

