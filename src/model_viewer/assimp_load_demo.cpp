// Program for testing Assimp model loading, as
// we build out some infrastructure to handle
// loading models with Assimp.
//
// Created by sean on 12/21/24.
// Our work is inspired by www.learnopengl.com.

#include "root_directory.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fmt/core.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Some model loading infrastructure.
// TODO: Will get moved to its own file(s).

struct Vertex {
  glm::vec3 mPosition;
  glm::vec2 mTextureCooords;
};

// Based heavily on Joey DeVries' model class.

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

private:
  // TODO: Add vector of meshes.
};

// -------------
// Program main.

int main() {
  auto path = std::string(project_root) + "/resources/learnopengl/backpack.obj";

  Model model{path};
}
