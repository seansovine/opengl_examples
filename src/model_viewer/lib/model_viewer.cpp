//
// Created by sean on 1/7/25.
//

// clang-format off
// Note, this must be included first.
#include "glad/glad.h"

#include "lib/model_viewer.h"
#include "tools/textured_mesh.h"

#include "models/models.h"

#include <learnopengl/filesystem.h>
// clang-format on

// -------------------
// Helper definitions.

bool configureGL(const Config& config) {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;

    return false; // Early return.
  }

  if (config.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  glEnable(GL_DEPTH_TEST);

  return true;
}

ShaderAndModels loadShaderAndModels() {
  std::string vertexShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer.vs");
  std::string fragmentShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer.fs");

  // Load shaders.
  auto ourShader = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

  // Load textures.
  auto texture1 = std::make_shared<GLTexture>("resources/textures/Bricks098_2K-JPG_Color.jpg", GL_RGB);
  auto texture2 = std::make_shared<GLTexture>("resources/learnopengl/textures/container.jpg", GL_RGB);

  if (!texture1->isLoaded() || !texture2->isLoaded()) {
    std::cout << "Failed to load texture" << std::endl;

    return {nullptr, nullptr, nullptr}; // Early return.
  }

  ourShader->use();
  // Load model
  auto model1 = std::make_shared<TexturedMesh>(texture2, models::cubeModel);
  auto model2 = std::make_shared<TexturedMesh>(texture1, models::planeModel);

  // These numbers just have to be unique.
  model1->bindTexture(0);
  model2->bindTexture(1);

  return {ourShader, model1, model2};
}

void setCallbacks(GLFWWrapper &window, Transformations &transformations) {
  // Set our resize callback that updates the projection.
  window.callbackInterface().mUserResizeCallback = [&window, &transformations](float, float) {
    transformations.updateProjectionTransformation(window.aspectRatio());
  };

  // Set the click-and-drag rotation callback.
  window.callbackInterface().mUserMouseDragCallback = [&transformations](double xAmt, double yAmt) {
    transformations.rotateViewTransformation(0.05 * yAmt, 0.05 * xAmt);
    transformations.updateViewTransformation();
  };

  // Set mouse wheel zoom callback.
  window.callbackInterface().mUserMouseScrollCallback = [&window, &transformations](double, double yDelta) {
    transformations.updateFoV(yDelta);
    transformations.updateProjectionTransformation(window.aspectRatio());
  };
}

void constantRotation(Transformations &transformations) {
  // Rotates the model by a fixed amount on each call.
  // Right now this is not tied to a clock, so the rotation
  // speed will be different on each system. (We'll fix that.)
  transformations.updateModelTransformation();
  // Rotate by constant amount around x- and y-axes.
  transformations.rotateViewTransformation(0.01, 0.0);
  transformations.updateViewTransformation();
}

// ----------------------------
// Call OpenGL rendering funcs.

void clearBuffers() {
  // Clear buffers. Use black background.
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
