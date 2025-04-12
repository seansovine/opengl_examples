// Class that wraps the GLFW interface for easy re-use.
// Adapted from code in De Vries' coordinate systems
// example from www.learnopengl.com.
//
// NOTE TO USER: `#include "glad/glad.h"` must come before this
// header in programs that use this utility.

#ifndef GLFW_WRAPPER_H
#define GLFW_WRAPPER_H

#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>

// -----------------------------------
// Interface for passing callbacks in.

class CallbackInterface {
  using ResizeCallback = std::function<void(int width, int height)>;
  using MouseDragCallback = std::function<void(double width, double height)>;
  using MouseScrollCallback = std::function<void(double xDelta, double yDelta)>;

public:
  CallbackInterface() = default;

  void resizeCallback(int width, int height) const {
    // Make sure the viewport matches the new window dimensions.
    glViewport(0, 0, width, height);

    // Call the user's resize callback, if any.
    if (mUserResizeCallback) {
      mUserResizeCallback(width, height);
    }
  }

  void cursorPositionClickAndDragCallback(double xpos, double ypos, bool leftButtonDown) const {
    static bool dragging = false;
    static double lastX;
    static double lastY;

    if (!dragging && leftButtonDown) {
      dragging = true;
      lastX = xpos;
      lastY = ypos;
    } else if (dragging && !leftButtonDown) {
      dragging = false;
    } else if (dragging) {
      if (mUserMouseDragCallback) {
        double deltaX = xpos - lastX;
        double deltaY = ypos - lastY;

        mUserMouseDragCallback(deltaX, deltaY);
      }

      lastX = xpos;
      lastY = ypos;
    }
  }

  void mouseScrollCallback(double xDelta, double yDelta) const {
    if (mUserMouseScrollCallback) {
      mUserMouseScrollCallback(xDelta, yDelta);
    }
  }

public:
  ResizeCallback mUserResizeCallback = {};
  MouseDragCallback mUserMouseDragCallback = {};
  MouseScrollCallback mUserMouseScrollCallback = {};
};

// ---------------------
// GLFW Wrapper Utility.

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

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window.
    mWindow = glfwCreateWindow(INIT_SCR_WIDTH, INIT_SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (mWindow == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return false;
    }

    mDimensions = {INIT_SCR_WIDTH, INIT_SCR_HEIGHT};

    glfwMakeContextCurrent(mWindow);

    // For use by GLFW callbacks.
    glfwSetWindowUserPointer(mWindow, this);

    // Set our GLFW callbacks.

    // Set resize callback.
    glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow *window, int width, int height) -> void {
      auto thisWindow = static_cast<GLFWWrapper *>(glfwGetWindowUserPointer(window));
      thisWindow->mDimensions = {width, height};
      thisWindow->mCallbackInterface.resizeCallback(width, height);
    });

    // Set callback to handle click and drag.
    glfwSetCursorPosCallback(mWindow, [](GLFWwindow *window, double xpos, double ypos) -> void {
      bool leftButtonDown = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
      auto thisWindow = static_cast<GLFWWrapper *>(glfwGetWindowUserPointer(window));
      thisWindow->mCallbackInterface.cursorPositionClickAndDragCallback(xpos, ypos, leftButtonDown);
    });

    // Set mouse scroll wheel callback.
    glfwSetScrollCallback(mWindow, [](GLFWwindow *window, double xDelta, double yDelta) -> void {
      auto thisWindow = static_cast<GLFWWrapper *>(glfwGetWindowUserPointer(window));
      thisWindow->mCallbackInterface.mouseScrollCallback(xDelta, yDelta);
    });

    return true;
  }

  [[nodiscard]] bool shouldClose() const { return glfwWindowShouldClose(mWindow); }

  void processInput() const {
    // Handle key presses, currently just escape.
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);
  }

  void swapBuffers() const { glfwSwapBuffers(mWindow); }

  static void pollEvents() { glfwPollEvents(); }

  [[nodiscard]] const std::pair<float, float> &dimensions() const { return mDimensions; }

  [[nodiscard]] float aspectRatio() const { return mDimensions.first / mDimensions.second; }

  CallbackInterface &callbackInterface() { return mCallbackInterface; }

private:
  GLFWwindow *mWindow = nullptr;

  CallbackInterface mCallbackInterface = {};

  // For efficient use in glm function calls.
  std::pair<float, float> mDimensions;

  // Initial window dimensions.
  static constexpr unsigned int INIT_SCR_WIDTH = 800;
  static constexpr unsigned int INIT_SCR_HEIGHT = 600;
};

#endif // GLFW_WRAPPER_H
