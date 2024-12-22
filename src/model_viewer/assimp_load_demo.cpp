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

int main() {
  auto path = std::string(project_root) + "/resources/learnopengl/backpack.obj";

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

  if (scene) {
    fmt::print("Loaded scene successfully with Assimp.");;
  }
}
