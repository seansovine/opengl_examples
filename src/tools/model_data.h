// Some model loading infrastructure for loading, storing, and drawing
// data for a model loaded with Assimp. Modeled very closely from the
// mesh class on www.learnopengl.com.
//
// Created by sean on 12/22/24.
//

#ifndef MODEL_DATA_H
#define MODEL_DATA_H

// clang-format off
#include "glad/glad.h"

#include "mesh_data.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fmt/core.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <learnopengl/shader_m.h>

#include <map>
// clang-format on

// --------------------------------------
// Model class -- holds a model's meshes.
//  Based heavily on Joey DeVries' model class.

unsigned int textureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {
public:
  /// Throws on failure to load.
  explicit Model(const std::string &path) {
    if (!load(path)) {
      throw std::runtime_error("Failed to load model.");
    }
  }

  // Draw each mesh.
  void draw(Shader &shader) {
    for (const auto &mesh : mMeshes) {
      mesh.draw(shader);
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

    mDirectory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);

    return true;
  }

  void processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

      mMeshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex = {};
      glm::vec3 vector;

      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.mPosition = vector;

      // We always use the first set of texture coords.
      if (mesh->mTextureCoords[0]) {
        glm::vec2 vec;
        vec.x = mesh->mTextureCoords[0][i].x;
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.mTextureCoords = vec;
      }

      vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace &face = mesh->mFaces[i];

      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }

    std::vector<Texture> textures;
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    { // NOTE: For testing and debugging.
      auto numDiffuseTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
      auto numSpecularTextures = material->GetTextureCount(aiTextureType_SPECULAR);
      // NOTE: We use De Vries' terminology for these two.
      auto numNormalTextures = material->GetTextureCount(aiTextureType_HEIGHT);
      auto numHeightTextures = material->GetTextureCount(aiTextureType_AMBIENT);

      constexpr bool EXTRA_DEBUG_OUT = false;
      if (EXTRA_DEBUG_OUT &&
          numDiffuseTextures + numSpecularTextures + numNormalTextures + numHeightTextures > 0) {
        fmt::print("We found some textures!\n");
      }
    }

    std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // Return new mesh.

    return {vertices, indices, textures};
  }

  std::vector<Texture> loadTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);

      if (mLoadedMeshPaths.contains(str.C_Str())) {
        const Texture &texture = mLoadedTextures[mLoadedMeshPaths[str.C_Str()]];
        mLoadedTextures.push_back(texture);
      } else {
        Texture texture;
        texture.id = textureFromFile(str.C_Str(), this->mDirectory);
        texture.type = typeName;
        texture.path = str.C_Str();

        textures.push_back(texture);
        mLoadedTextures.push_back(texture);
        mLoadedMeshPaths[str.C_Str()] = mLoadedTextures.size() - 1;
      }
    }

    return textures;
  }

private:
  std::vector<Mesh> mMeshes;
  std::map<std::string, std::size_t> mLoadedMeshPaths;
  std::vector<Texture> mLoadedTextures;
  std::string mDirectory;
};

#endif // MODEL_DATA_H
