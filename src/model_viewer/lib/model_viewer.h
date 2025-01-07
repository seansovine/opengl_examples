//
// Created by sean on 1/7/25.
//

#ifndef MODEL_VIEWER_H
#define MODEL_VIEWER_H

// clang-format off
#include "lib/textured_mesh.h"

#include <learnopengl/shader_m.h>
#include <tools/glfw_wrapper.h>
#include <tools/transformations.h>
// clang-format on

// ---------------------
// Configuration params.

struct Config {
  bool wireframe = true;
  bool constantRotation = false;
};

// --------------------
// Helper declarations.

class TexturedMesh;

struct ShaderAndModels {
  std::shared_ptr<Shader> mShader;

  std::shared_ptr<TexturedMesh> mModel1;
  std::shared_ptr<TexturedMesh> mModel2;

  [[nodiscard]] bool isOkay() const { return mShader && mModel1 && mModel2; }
};

ShaderAndModels loadShaderAndModels();

void setCallbacks(GLFWWrapper &window, Transformations &transformations);

bool configureGL(const Config &config);
void clearBuffers();

void constantRotation(Transformations &transformations);

#endif // MODEL_VIEWER_H
