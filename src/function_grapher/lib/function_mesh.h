//
// Created by sean on 1/5/25.
//

#ifndef FUNCTION_MESH_H
#define FUNCTION_MESH_H

// clang-format off
#include "model_viewer/models/models.h"

#include <fmt/core.h>

#include <learnopengl/shader_m.h>
#include <tools/textured_mesh.h>
// clang-format on

// ------------------
// Geometric helpers.

struct Square {
  float mTopLeft[2];
  float mBtmRight[2];
};

// --------------------
// Function mesh class.

// Builds a mesh for graphing a function z = f(x, y).
// We will start very simple and gradually add features.

class FunctionMesh {
  using F = double (*)(double, double);

public:
  explicit FunctionMesh(const F func) : mF(func) { generateMesh(); }

  void generateMesh() {
    buildFloorMesh();

    computeFloorVertices();
    mFloorMesh = TexturedMesh(nullptr, mFloorMeshVertices);

    computeFunctionMeshVertices();
    mFunctionMesh = TexturedMesh(nullptr, mFunctionMeshVertices);
  }

  void printMeshData() const {
    // TODO: Printout some useful information on our mesh for debugging.
    fmt::print("Number of triangles, etc...");
  }

  void draw(Shader *shader) const {
    mFloorMesh.draw(shader);
    mFunctionMesh.draw(shader);
  }

  std::vector<float> &floorVertices() { return mFloorMeshVertices; }
  std::vector<float> &functionVertices() { return mFunctionMeshVertices; }

private:
  void buildFloorMesh() {
    mFloorMeshSquares.reserve(mNumCells * mNumCells);

    const double width = 1.0 / mNumCells;

    for (int i = 1; i <= mNumCells; i++) {
      for (int j = 1; j <= mNumCells; j++) {
        Square square;

        square.mTopLeft[0] = (i - 1) * width;
        square.mTopLeft[1] = (j - 1) * width;

        square.mBtmRight[0] = i * width;
        square.mBtmRight[1] = j * width;

        mFloorMeshSquares.push_back(square);
      }
    }
  }

  void computeFloorVertices() {
    auto vertices = std::vector<float>{};
    vertices.reserve(mFloorMeshSquares.size() * 10);

    for (const auto &square : mFloorMeshSquares) {
      // clang-format off
      // First triangle.
      vertices.insert(vertices.end(), {
        square.mTopLeft[0],
        0.0, // z = 0
        square.mTopLeft[1],
        0.0, // unused texture coord
        0.0, // unused texture coord
      });
      vertices.insert(vertices.end(), {
        square.mTopLeft[0],
        0.0, // z = 0
        square.mBtmRight[1],
        0.0, // unused texture coord
        0.0, // unused texture coord
      });
      vertices.insert(vertices.end(), {
        square.mBtmRight[0],
        0.0, // z = 0
        square.mTopLeft[1],
        0.0, // unused texture coord
        0.0, // unused texture coord
      });

      // Second triangle.
      vertices.insert(vertices.end(), {
        square.mBtmRight[0],
        0.0, // z = 0
        square.mBtmRight[1],
        0.0, // unused texture coord
        0.0, // unused texture coord
      });
      vertices.insert(vertices.end(), {
        square.mBtmRight[0],
        0.0, // z = 0
        square.mTopLeft[1],
        0.0, // unused texture coord
        0.0, // unused texture coord
      });
      vertices.insert(vertices.end(), {
        square.mTopLeft[0],
        0.0, // z = 0
        square.mBtmRight[1],
        0.0, // unused texture coord
        0.0, // unused texture coord
      });
      // clang-format on
    }

    mFloorMeshVertices = vertices;
  }

  void computeFunctionMeshVertices() {
    auto vertices = std::vector<float>{};
    vertices.reserve(mFloorMeshVertices.size());

    // Now update y-coordinates w/ function values.
    for (int i = 0; i < mFloorMeshVertices.size(); i += 5) {
      float x = mFloorMeshVertices[i + 0];
      float z = mFloorMeshVertices[i + 2];
      // float y = mFloorMeshVertices[i + 1];
      float y = static_cast<float>(mF(x, z));

      vertices.insert(vertices.end(), {x, y, z, 0.0, 0.0});
    }

    mFunctionMeshVertices = vertices;
  }

private:
  // Function z = mF(x, y) that we will graph.
  F mF;

  // Width of cells to divide x,y plane region into.
  static constexpr int mNumCells = 100;
  // Squares that make up x,y-plane mesh.
  std::vector<Square> mFloorMeshSquares = {};
  // Vertices of triangular tessellation from above squares.
  std::vector<float> mFloorMeshVertices = {};
  // Vertices of triangular tessellation from above squares.
  std::vector<float> mFunctionMeshVertices = {};
  // Default is uninitialized.
  TexturedMesh mFloorMesh{};
  // Default is uninitialized.
  TexturedMesh mFunctionMesh{};
};

#endif // FUNCTION_MESH_H
