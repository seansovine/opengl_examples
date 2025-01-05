//
// Created by sean on 1/5/25.
//

#ifndef FUNCTION_MESH_H
#define FUNCTION_MESH_H

// Manually order includes for now.
// clang-format off
#include <fmt/core.h>

#include <learnopengl/shader_m.h>
// clang-format on

// Builds a mesh for graphing a function z = f(x, y).
// We will start very simple and gradually add features.

class FunctionMesh {
  using F = double (*)(double, double);

public:
  explicit FunctionMesh(const F func) : mF(func) { generateMesh(); }

  void generateMesh() {
    // TODO: Build a triangular mesh for graphing mF.
  }

  void printMeshData() const {
    // TODO: Printout some useful information on our mesh for debugging.
    fmt::print("Number of triangles, etc...");
  }

  void draw(Shader &shader) const {
    // TODO: Add OpenGL code to draw the function graph.
  }

private:
  // We will graph z = mF(x, y).
  F mF;

  // Width of cells to divide x,y plane region into.
  static constexpr double mCellWidth = 1e-2;
};

#endif // FUNCTION_MESH_H
