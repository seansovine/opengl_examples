// Loads model vertices and vertex attribute pointers
// from a given array with entries in groups of 5, with
// coords for each vertex listed as x, y, z, u, v.
//
// Created by sean on 12/4/24.

#ifndef TEXTURED_MODEL_H
#define TEXTURED_MODEL_H

// clang-format off
#include "glad/glad.h"

#include <learnopengl/shader_m.h>
#include <tools/gl_texture.h>

#include <memory>
#include <vector>
// clang-format on

// -------------
// TexturedModel

// Represents a model with x, y, z coords followed by u, v texture coords.

class TexturedMesh {
public:
  TexturedMesh() = default;

  /// Takes ownership of texture.
  explicit TexturedMesh(const std::shared_ptr<GLTexture> &texture, const std::vector<float> &model) {
    const float *vertices = model.data();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);

    // Put vertex data in buffer
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(float), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    mCount = static_cast<int>(std::size(model) / 5);
    mTexture = texture;
  }

  TexturedMesh(const TexturedMesh &) = delete;

  ~TexturedMesh() {
    // de-allocate now
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
  }

  [[nodiscard]] unsigned int VAO() const { return mVAO; }

  [[nodiscard]] int vertexCount() const { return mCount; }

  void bindTexture(int textureNum) {
    if (!mTexture) {
      throw std::runtime_error("TexturedMesh instance is uninitialized: Cannot bind texture.");
    }

    mTextureNum = textureNum;
    mTexture->bind(GL_TEXTURE0 + textureNum);
  }

  // Draw my triangles.

  void draw(Shader *shader) const {
    if (mTexture) {
      // Pass our texture number as uniform on shader.
      shader->setInt("texture1", mTextureNum);
    }

    // Bind VAO.
    glBindVertexArray(mVAO);

    // Draw the model.
    glDrawArrays(GL_TRIANGLES, 0, mCount);
  }

private:
  unsigned int mVAO = 0;
  unsigned int mVBO = 0;
  int mCount = 0;

  int mTextureNum = -1;
  std::shared_ptr<GLTexture> mTexture;
};

#endif // TEXTURED_MODEL_H
