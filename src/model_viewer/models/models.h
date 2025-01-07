//
// Created by sean on 1/6/25.
//

#ifndef MODELS_H
#define MODELS_H

#include <vector>

namespace models {
// For now, we'll hardcode De Vries' box model, but later
// we'll add ability to load other models, maybe with Assimp.

// Vertices here are listed as (x,y,z,u,v), where u,v are texture coords.

// clang-format off
const std::vector<float> cubeModel = {
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,   0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,   0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

  -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,   -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

  -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,   -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,   0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,   0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,   0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
  0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

  -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,   0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,   -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,
};

const std::vector<float> planeModel = {
  -5.0, -3.0, -5.0, 0.0, 0.0,  5.0, -3.0, -5.0, 1.0, 0.0,  -5.0, -3.0, 5.0, 0.0, 1.0,
   5.0, -3.0,  5.0, 1.0, 1.0,  5.0, -3.0, -5.0, 1.0, 0.0,  -5.0, -3.0, 5.0, 0.0, 1.0,
};
// clang-format on

} // namespace models

#endif // MODELS_H
