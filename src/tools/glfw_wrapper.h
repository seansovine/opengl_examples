// Class that wraps the GLFW interface for easy re-use.
// Adapted from code in De Vries' coordinate systems
// example from www.learnopengl.com.
//
// NOTE TO USER: #include "glad/glad.h must come before this.
//

#ifndef GLFW_WRAPPER_H
#define GLFW_WRAPPER_H

#include <GLFW/glfw3.h>

#include <iostream>

// GLFW Wrapper

class GLFWWrapper {
public:
  GLFWWrapper() = default;

  ~GLFWWrapper() {
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
  }

  bool init() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
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

  bool shouldClose() {
    return glfwWindowShouldClose(mWindow);
  }

  void processInput() {
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);
  }

  void swapBuffers() {
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(mWindow);
  }

  static void pollEvents() {
    glfwPollEvents();
  }

  [[nodiscard]] const std::pair<float, float>& dimensions() const {
    return mDimensions;
  }

  float aspectRatio() const {
    return mDimensions.first / mDimensions.second;
  }

private:
  GLFWwindow *mWindow = nullptr;

  // For efficient use in glm function calls.
  std::pair<float, float> mDimensions;

  // Initial window dimensions.
  static constexpr unsigned int SCR_WIDTH = 800;
  static constexpr unsigned int SCR_HEIGHT = 600;

  // glfw: whenever the window size changed (by OS or user resize) this callback function executes
  // ---------------------------------------------------------------------------------------------
  static void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
  }
};

#endif //GLFW_WRAPPER_H
