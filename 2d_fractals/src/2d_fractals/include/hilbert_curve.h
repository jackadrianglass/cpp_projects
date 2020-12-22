//
// Created by Jack Glass on 2020-09-30.
//

#ifndef MYPROJECT_HILBERT_CURVE_H
#define MYPROJECT_HILBERT_CURVE_H

#include <vector>
#include <glm/vec3.hpp>

namespace fractals {

auto hilbert_curve(int iter) -> std::vector<glm::vec3>;
auto hilbert_curve_algo(unsigned int iter, int length) -> glm::vec3;

}


#endif//MYPROJECT_HILBERT_CURVE_H
