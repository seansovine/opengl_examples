// Structures for loading, storing, and drawing
// data for a model loaded with Assimp.
//
// Created by sean on 12/22/24.
//

#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include "glad/glad.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fmt/core.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <learnopengl/shader_m.h>

// Some model loading infrastructure.
// TODO: Will get moved to its own file(s).

// ------------------
// Vertex POD struct.

struct Vertex {
  glm::vec3 mPosition;
  glm::vec2 mTextureCooords;
};

// ---------------------------------------
// Mesh class -- for now without textures.
//  Based heavily on Joey DeVries' mesh class.

class Mesh {
public:
  Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
      : mVertices{vertices}, mIndices{indices} {
    setup();
  }

  void Draw(Shader &) {
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // TODO: When we add textures, activate them here.

    // Reset bound texture, though we aren't using them yet.
    glActiveTexture(GL_TEXTURE0);
  }

private:
  void setup() {
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // NOTE: This is assuming that our struct and the glm types
    // are laid out in memory sequentially with no padding.
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mVertices.size()) * sizeof(Vertex), &mVertices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(mIndices.size()) * sizeof(unsigned int),
                 &mIndices[0], GL_STATIC_DRAW);

    // TODO: Here we only bind vertices. Later add textures.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));
    glBindVertexArray(0);
  }

private:
  std::vector<Vertex> mVertices;
  std::vector<unsigned int> mIndices;

  unsigned int mVAO = 0;
  unsigned int mVBO = 0;
  unsigned int mEBO = 0;
};

// --------------------------------------
// Model class -- holds a model's meshes.
//  Based heavily on Joey DeVries' model class.

class Model {
public:
  /// Throws on failure to load.
  explicit Model(const std::string &path) {
    if (!load(path)) {
      throw std::runtime_error("Failed to load model.");
    }
  }

private:
  bool load(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene && scene->mRootNode && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
      fmt::print("Loaded scene successfully with Assimp.\n");
    } else {
      return false;
    }

    processNode(scene->mRootNode, scene);

    return true;
  }

  void processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      // TODO: Create mesh and store in vector.
      processMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  void processMesh(aiMesh *mesh, const aiScene *scene) {
    // TODO: Lots of stuff goes here...

    {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

      auto numDiffuseTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
      auto numSpecularTextures = material->GetTextureCount(aiTextureType_SPECULAR);
      // NOTE: We use De Vries' terminology for these two.
      auto numNormalTextures = material->GetTextureCount(aiTextureType_HEIGHT);
      auto numheightTextures = material->GetTextureCount(aiTextureType_AMBIENT);

      if (numDiffuseTextures + numSpecularTextures + numNormalTextures + numheightTextures > 0) {
        fmt::print("We found some textures!\n");
      }
    }
  }

private:
  std::vector<Mesh> mMeshes;
  std::vector<unsigned int> mIndices;
};

#endif // MODEL_DATA_H
