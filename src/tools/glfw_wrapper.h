// Class that wraps the GLFW interface for easy re-use.
// Adapted from code in De Vries' coordinate systems
// example from www.learnopengl.com.
//
// NOTE TO USER: `#include "glad/glad.h"` must come before this
// header in programs that use this utility.

#ifndef GLFW_WRAPPER_H
#define GLFW_WRAPPER_H

#include <GLFW/glfw3.h>

#include <iostream>

// GLFW Wrapper

class GLFWWrapper {
public:
  GLFWWrapper() = default;

  ~GLFWWrapper() {
    // Terminate GLFW and release its resources
    glfwTerminate();
  }

  bool init() {
    // Initialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // NOTE: Adding this until we get the resize callback
    // properly integrated with or projection matrix code.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create window.
    mWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (mWindow == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return false;
    }

    mDimensions = {SCR_WIDTH, SCR_HEIGHT};

    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

    return true;
  }

  [[nodiscard]] bool shouldClose() const {
    return glfwWindowShouldClose(mWindow);
  }

  void processInput() const {
    // Handle key presses, currently just escape.
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);
  }

  void swapBuffers() const {
    glfwSwapBuffers(mWindow);
  }

  static void pollEvents() {
    glfwPollEvents();
  }

  [[nodiscard]] const std::pair<float, float>& dimensions() const {
    return mDimensions;
  }

  [[nodiscard]] float aspectRatio() const {
    return mDimensions.first / mDimensions.second;
  }

private:
  GLFWwindow *mWindow = nullptr;

  // For efficient use in glm function calls.
  std::pair<float, float> mDimensions;

  // Initial window dimensions.
  static constexpr unsigned int SCR_WIDTH = 800;
  static constexpr unsigned int SCR_HEIGHT = 600;

  // Callback that GLFW calls after window is resized.
  static void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height) {
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
  }
};

#endif //GLFW_WRAPPER_H
