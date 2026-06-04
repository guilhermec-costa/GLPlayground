#include <glad/glad.h>
#include "gl_utils.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <optional>

static void processInput(GLFWwindow* window);

void helloTriangle(GLFWwindow* window, std::optional<InputProcessor>);
void trianglePlayground(GLFWwindow* window, std::optional<InputProcessor>);
void triangleVBOS(GLFWwindow* window, std::optional<InputProcessor>);

void framebufferResizeCallback(GLFWwindow* window, int width, int height);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

int main() {

  if (glfwInit() == GLFW_FALSE) {
    std::cout << "failed to initialize GLFW\n";
    return -1;
  };

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLIntro", NULL, NULL);
  if (!window) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialized GLAD\n";
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

  // helloTriangle(window);
  // trianglePlayground(window, processInput);
  triangleVBOS(window, processInput);

  glfwTerminate();
  return 0;
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  std::cout << "resizing window to w: " << width << " h: " << height << "\n";
  glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
};