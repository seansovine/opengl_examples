// Program for testing Assimp model loading, as
// we build out some infrastructure to handle
// loading models with Assimp.
//
// Created by sean on 12/21/24.
// Our work is inspired by www.learnopengl.com.

#include "lib/model_data.h"
#include "root_directory.h"

#include <fmt/core.h>

// -------------
// Program main.

int main() {
  auto path = std::string(project_root) + "/resources/learnopengl/backpack.obj";

  Model model{path};
}
