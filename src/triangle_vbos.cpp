#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <string>
#include <vector>

#include "gl_utils.h"
#include "triangle.h"

static std::string vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

static std::string fragmentShaderSource = R"(
#version 330 core

out vec4 fragColor;

void main() {
  fragColor = vec4(0.5f, 0.5f, 0.4f, 1.0f);
}
)";

void triangleVBOS(GLFWwindow* window, std::optional<InputProcessor> inputProcessor) {
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

  const char* vss = vertexShaderSource.c_str();
  glShaderSource(vertexShader, 1, &vss, NULL);
  glCompileShader(vertexShader);
  if (checkShaderCompilation(vertexShader) == -1) {
    return;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fss = fragmentShaderSource.c_str();
  glShaderSource(fragmentShader, 1, &fss, NULL);
  glCompileShader(fragmentShader);

  if (checkShaderCompilation(fragmentShader) == -1) {
    return;
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  if (checkProgramLinkingStatus(shaderProgram) == -1) {
    return;
  }

  float triangleOneVertexData[] = { 
    0.0f, 0.3f, 0.0f, 
    -0.5f, -0.2f, 0.0f, 
    0.5f, -0.5f, 0.0f
  };

  Triangle t1 = Triangle(triangleOneVertexData, sizeof(triangleOneVertexData));

  float triangleTwoVertexData[] = { 
    0.0f, 0.5f, 0.0f, 
    -0.5f, -0.5f, 0.0f, 
    0.5f, -0.5f, 0.0f
  };
  Triangle t2 = Triangle(triangleTwoVertexData, sizeof(triangleTwoVertexData));

  std::vector<Triangle> triangles;
  triangles.emplace_back(triangleOneVertexData, sizeof(triangleOneVertexData));
  triangles.emplace_back(triangleTwoVertexData, sizeof(triangleTwoVertexData));

  for (auto& t : triangles) {
    t.make();
  }

  while (!glfwWindowShouldClose(window)) {
    if (inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.2470f, 0.1333f, 0.0588f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    for (auto& t : triangles) {
      t.bind();
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);
}