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
  explicit FunctionMesh(const F func) : mFunc(func) { generateMesh(); }

  void generateMesh() {
    buildFloorMesh();
    computeFloorMeshVertices();
    computeFunctionMeshVertices();

    mFloorMesh = std::make_shared<TexturedMesh>(nullptr, mFloorMeshVertices);
    mFunctionMesh = std::make_shared<TexturedMesh>(nullptr, mFunctionMeshVertices);
  }

  void printMeshData() const {
    // Turn off console output buffering to see results immediately.
    setbuf(stdout, nullptr);
    // Print out some useful information on our mesh for debugging.
    fmt::print("Number of triangles: {}", mFloorMeshSquares.size() * 2);
  }

  void draw(Shader *shader) const {
    // NOTE: This makes assumptions about the shader it's used with.
    shader->setVec4("rgbaColor", glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
    mFloorMesh->draw(shader);
    shader->setVec4("rgbaColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    mFunctionMesh->draw(shader);
  }

  std::vector<float> &floorVertices() { return mFloorMeshVertices; }
  std::vector<float> &functionVertices() { return mFunctionMeshVertices; }

  TexturedMesh &floorMesh() { return *mFloorMesh; }
  TexturedMesh &functionMesh() { return *mFunctionMesh; }

private:
  void buildFloorMesh() {
    mFloorMeshSquares.reserve(mNumCells * mNumCells);
    const double width = 1.0 / mNumCells;

    for (int i = 1; i <= mNumCells; i++) {
      for (int j = 1; j <= mNumCells; j++) {
        // clang-format off
        Square square{
          .mTopLeft ={
            static_cast<float>((i - 1) * width),
            static_cast<float>((j - 1) * width),
          },
          .mBtmRight = {
            static_cast<float>(i * width),
            static_cast<float>(j * width),
          }
        };
        // clang-format on
        mFloorMeshSquares.push_back(square);
      }
    }
  }

  void computeFloorMeshVertices() {
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

    mFloorMeshVertices = std::move(vertices);
  }

  void computeFunctionMeshVertices() {
    auto vertices = std::vector<float>{};
    vertices.reserve(mFloorMeshVertices.size());

    // Now update y-coordinates w/ function values.
    for (std::size_t i = 0; i < mFloorMeshVertices.size(); i += 5) {
      float x = mFloorMeshVertices[i + 0];
      float z = mFloorMeshVertices[i + 2];

      auto y = static_cast<float>(mFunc(x, z));

      vertices.insert(vertices.end(), {x, y, z, 0.0, 0.0});
    }

    mFunctionMeshVertices = std::move(vertices);
  }

private:
  // The function z = mF(x, y) that we will graph.
  F mFunc;

  // Number of subdivisions of x,y axes when creating cells.
  static constexpr int mNumCells = 100;

  // Squares that make up x,y-plane mesh.
  std::vector<Square> mFloorMeshSquares = {};
  // Vertices of triangular tessellation built from squares.
  std::vector<float> mFloorMeshVertices = {};
  // Tessellation vertices with heights from function values.
  std::vector<float> mFunctionMeshVertices = {};

  // Default is uninitialized.
  std::shared_ptr<TexturedMesh> mFloorMesh{};
  // Default is uninitialized.
  std::shared_ptr<TexturedMesh> mFunctionMesh{};
};

#endif // FUNCTION_MESH_H
