// The start of a utility for graphing functions of the form z = f(x, y).
//
// Created by sean on 1/5/25.
//

// Manually order includes for now.
// clang-format off

// Note, this should be included first.
#include "glad/glad.h"

#include "lib/function_mesh.h"

#include <fmt/core.h>
// clang-format on

int main() {
  fmt::print("Hello, World from the future function grapher!\n");

  // A simple function for testing mesh generation.
  auto func = [](double x, double y) -> double { return x * x + y * y; };

  FunctionMesh mesh{func};

  // Print out some info on the generated mesh.
  mesh.printMeshData();

  // TODO: Add basic OpenGL + GLFW setup.
  // Add render loop code that calls mesh.draw(shader).
}
