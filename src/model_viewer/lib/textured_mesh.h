// Loads model vertices and vertex attribute pointers
// from a given array with entries in groups of 5, with
// coords for each vertex listed as x, y, z, u, v.
//
// Created by sean on 12/4/24.

#ifndef TEXTURED_MODEL_H
#define TEXTURED_MODEL_H

#include "glad/glad.h"

#include <tools/gl_texture.h>

#include <memory>

// For now, we'll hardcode De Vries' box model, but later
// we'll add ability to load other models, maybe with Assimp.

// clang-format off
static constexpr float sModelXYZUVVertices[] = {
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
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,   -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f
};
// clang-format on

// -------------
// TexturedModel

// Represents a model with x, y, z coords followed by u, v texture coords.

class TexturedMesh {
public:
  /// Takes ownership of texture.
  explicit TexturedMesh(std::shared_ptr<GLTexture>&& texture) {
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);

    // Put vertex data in buffer
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sModelXYZUVVertices), sModelXYZUVVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    mCount = sizeof(sModelXYZUVVertices) / (5 * sizeof(float));
    mTexture = texture;
  }

  TexturedMesh(const TexturedMesh&) = delete;

  ~TexturedMesh() {
    // de-allocate now
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
  }

  [[nodiscard]] unsigned int VAO() const {
    return mVAO;
  }

  [[nosdiscard]] int vertexCount() const {
    return mCount;
  }

  void bindTexture(GLenum textureUnit) const {
    mTexture->bind(textureUnit);
  }

private:
  unsigned int mVAO = 0;
  unsigned int mVBO = 0;
  int mCount = 0;

  std::shared_ptr<GLTexture> mTexture;
};

#endif //TEXTURED_MODEL_H
