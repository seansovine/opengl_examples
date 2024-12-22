//
// Created by sean on 12/10/24.
//

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_m.h>

#include <algorithm>
#include <memory>

// ----------------
// Transformations.

// Class representing coordinate transformations that
// are passed to a shader via uniform variables.

class Transformations {
public:
  Transformations(const std::shared_ptr<Shader> &shader, float aspectRatio) : mShader(shader) {
    setupMatrices(aspectRatio);
  }

  void updateProjectionTransformation(float aspectRatio) {
    mProjectionMatrix = glm::perspective(glm::radians(static_cast<float>(mFoV)), aspectRatio, 0.1f, 100.0f);
    mShader->setMat4("projection", mProjectionMatrix);
  }

  void updateFoV(double delta) {
    mFoV -= delta;

    constexpr double FOV_MIN = 1.0;
    constexpr double FOV_MAX = 60.0;

    mFoV = std::clamp(mFoV, FOV_MIN, FOV_MAX);
  }

  void updateModelTransformation() {
    constexpr float increment = 0.4f;
    constexpr unsigned int mod = 360 / increment;
    static unsigned int step = 0;

    // Update model matrix.

    float angle = 20.0f + increment * step;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    mModelMatrix = glm::mat4(1.0f);
    makeModelMatrix(mModelMatrix, position, angle);
    mShader->setMat4("model", mModelMatrix);

    // Update "time" step.
    step = (step + 1) % mod;
  }

  void updateViewTransformation() {
    // Update view matrix.

    mViewMatrix = glm::mat4(1.0f);
    mViewMatrix = glm::translate(mViewMatrix, mCameraPosition) * glm::inverse(mViewRotation);
    mShader->setMat4("view", mViewMatrix);
  }

  void rotateViewTransformation(float xAngle, float yAngle) {
    mViewRotation = glm::rotate(mViewRotation, -xAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    mViewRotation = glm::rotate(mViewRotation, -yAngle, glm::vec3(0.0f, 1.0f, 0.0f));
  }

private:
  // NOTE: Assumes we don't resize the window.
  // We could have the window resize callback call this to
  // update the aspect ratio in the perspective transformation.
  void setupMatrices(const float aspectRatio) {
    // Matrices are based on www.learnopengl.com coordinate systems example.

    // Performs perspective projection.
    mProjectionMatrix = glm::perspective(glm::radians(static_cast<float>(mFoV)), aspectRatio, 0.1f, 100.0f);

    // Converts world coordinates to camera viewpoint coordinates.
    mViewMatrix = glm::translate(mViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

    float angle = 20.0f;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    // Converts model local coordinates to world coordinates.
    makeModelMatrix(mModelMatrix, position, angle);

    // Pass matrices to shader as uniforms.
    mShader->setMat4("projection", mProjectionMatrix);
    mShader->setMat4("view", mViewMatrix);
    mShader->setMat4("model", mModelMatrix);
  }

  // Helper for constructing the model matrix.
  void makeModelMatrix(glm::mat4 &model, const glm::vec3 &position, float angle) const {
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(mScaleFactor, mScaleFactor, mScaleFactor));
  }

private:
  std::shared_ptr<Shader> mShader;

  // Camera field of view.
  double mFoV = 45.0f;
  // Model scale factor.
  float mScaleFactor = 0.2f;

  glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
  glm::mat4 mViewMatrix = glm::mat4(1.0f);
  glm::mat4 mModelMatrix = glm::mat4(1.0f);

  glm::vec3 mCameraPosition = glm::vec3(0.0f, 0.0f, -3.0f);
  glm::mat4 mViewRotation = glm::mat4(1.0f);
};

#endif // TRANSFORMATIONS_H
