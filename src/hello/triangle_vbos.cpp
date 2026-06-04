#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <string>
#include <vector>

#include "gl_utils.h"
#include "triangle.h"

static std::string t1VSS = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

static std::string t1FSS = R"(
#version 330 core
out vec4 fragColor;

void main() {
  fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)";

static std::string t2VSS = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

static std::string t2FSS = R"(
#version 330 core

out vec4 fragColor;

void main() {
  fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
)";

void triangleVBOS(GLFWwindow* window, std::optional<InputProcessor> inputProcessor) {
  float triangleOneVertexData[] = { 
    0.7f, 0.5f, 0.0f, 
    0.6f, 0.2f, 0.0f, 
    0.8f, 0.2f, 0.0f
  };

  float triangleTwoVertexData[] = { 
    0.0f, 0.5f, 0.0f, 
    -0.5f, -0.5f, 0.0f, 
    0.5f, -0.5f, 0.0f
  };

  std::vector<Triangle> triangles;

  auto p1 = createShaderProgram(t1VSS, t1FSS);
  auto p2 = createShaderProgram(t2VSS, t2FSS);
  triangles.emplace_back(triangleOneVertexData, sizeof(triangleOneVertexData), p1);
  triangles.emplace_back(triangleTwoVertexData, sizeof(triangleTwoVertexData), p2);

  for (auto& t : triangles) {
    t.make();
  }

  while (!glfwWindowShouldClose(window)) {
    if (inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& t : triangles) {
      t.draw();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}