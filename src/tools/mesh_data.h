// Holds the data needed to render a textured mesh. Modeled
// very closely from the mesh class on www.learnopengl.com.
//
// Created by sean on 12/27/24.
//

#ifndef MESH_DATA_H
#define MESH_DATA_H

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <learnopengl/shader_m.h>

#include <string>
#include <vector>

// ------------------------------
// Vertex and texture POD struct.

struct Vertex {
  glm::vec3 mPosition;
  glm::vec2 mTextureCoords;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

// ---------------------------------------
// Mesh class -- for now without textures.
//  Based heavily on Joey DeVries' mesh class.

class Mesh {
public:
  Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
       const std::vector<Texture> &textures)
      : mVertices{vertices}, mIndices{indices}, mTextures{textures} {
    setup();
  }

  void draw(Shader &shader) const {
    // Bind my VAO and draw it.
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, nullptr);

    {
      // NOTE: We're only using diffuse right now, but
      // later we will add the other types, so we leave
      // the code hear to load those types.
      unsigned int diffuseNr = 1;
      unsigned int specularNr = 1;
      unsigned int normalNr = 1;
      unsigned int heightNr = 1;

      // Activate textures here.
      for (unsigned int i = 0; i < mTextures.size(); i++) {
        // Activate appropriate loaded texture.
        glActiveTexture(GL_TEXTURE0 + i);

        // We use these to build name of uniform.
        std::string number;
        std::string name = mTextures[i].type;

        if (name == "texture_diffuse")
          number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
          number = std::to_string(specularNr++);
        else if (name == "texture_normal")
          number = std::to_string(normalNr++);
        else if (name == "texture_height")
          number = std::to_string(heightNr++);

        // Assign texture unit as uniform.
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
      }
    }

    // Unbind this VAO.
    glBindVertexArray(0);
    // Reset bound texture.
    glActiveTexture(GL_TEXTURE0);
  }

private:
  void setup() {
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);

    // NOTE: This is assuming that our struct and the glm
    // types are laid out in memory sequentially with no padding.

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mVertices.size()) * sizeof(Vertex), &mVertices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(mIndices.size()) * sizeof(unsigned int),
                 &mIndices[0], GL_STATIC_DRAW);

    // TODO: Here we bind vertex and diffuse texture coordinates.
    // If we add other texture types, we need to create VAA's here.

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, mTextureCoords));

    // Unbind this VAO since we're done.
    glBindVertexArray(0);
  }

private:
  std::vector<Vertex> mVertices;
  std::vector<unsigned int> mIndices;
  std::vector<Texture> mTextures;

  unsigned int mVAO = 0;
  unsigned int mVBO = 0;
  unsigned int mEBO = 0;
};

#endif // MESH_DATA_H
