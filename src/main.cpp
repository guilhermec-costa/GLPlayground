#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  std::cout << "resizing window to w: " << width << " h: " << height << "\n";
  glViewport(0, 0, width, height);
};

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

/*
  Shader:
    program that runs on the GPU during the graphics pipeline.

  Graphics Pipeline:

    Vertex Shader:
      runs once for each vertex.
    ↓

    Primitive Assembly (pre-geometry):
      groups vertices into primitives specified by the draw call
      (GL_POINTS, GL_LINES, GL_TRIANGLES, etc.)
    ↓

    Geometry Shader (optional):
      runs once for each primitive assembled previously.

      It can:
      - generate new vertices
      - discard primitives
      - define a new primitive interpretation through `layout(... ) out`
    ↓

    Primitive Assembly (post-geometry / shape assembly):
      groups vertices emitted by the geometry shader
      according to the geometry shader output primitive.

      If no geometry shader exists:
        interprets vertices using the primitive type from the draw call.
    ↓

    Rasterization:
      converts primitives into fragments.

      Determines which screen samples/pixels are covered by the primitive.

      Outputs fragments (not pixels yet).
    ↓

    Clipping:
      discards portions of primitives outside the viewing volume.
    ↓


    Fragment:
      contains all data required to shade a potential pixel
      (interpolated attributes, depth, texture coordinates, etc.)
    ↓

    Fragment Shader:
      runs once for each fragment.

      Its purpose is to compute the fragment output
      (usually color, optionally depth/discard).
    ↓

    Per-fragment operations:
      depth testing
      stencil testing
      blending
      etc.
    ↓

    Final framebuffer output
*/

int main() {
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "failed to initialize GLFW\n";
    return -1;
  };

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLIntro", NULL, NULL);
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

  // list of 3D coordinates
  float vertices[] = {
      -0.5, -0.5f, 0.0f, // vertex
      0.5,  -0.5f, 0.0f  // vertex
  };

  while (!glfwWindowShouldClose(window)) {
    // frame start
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
    // frame end
  }

  glfwTerminate();
  return 0;
}