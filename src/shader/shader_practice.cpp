#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <optional>
#include <string>
#include <vector>

#include "gl_utils.h"
#include "triangle.h"

static std::string t1VSS = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

void main() {
  gl_Position = vec4(aPos, 1.0);
}
)";

static std::string t1FSS = R"(
#version 330 core

out vec4 FragColor;

uniform vec4 ourColor;

void main() {
  FragColor = ourColor; 
}
)";

void shaderPractice(GLFWwindow *window,
                    std::optional<InputProcessor> inputProcessor) {
  float triangleOneVertexData[] = {0.0f, 0.5f, 0.0f,  -0.5f, -0.5f,
                                   0.0f, 0.5f, -0.5f, 0.0f};

  std::vector<Triangle> triangles;

  auto program = createShaderProgram(t1VSS, t1FSS);
  triangles.emplace_back(triangleOneVertexData, sizeof(triangleOneVertexData),
                         program);

  for (auto &t : triangles) {
    t.make();
  }

  // returns the uniform location WITHIN this program. It is not a unique/global
  // value. It is a "program-local location" WITHIN THE SPECIFIED PROGRAM
  int vertexColorLocation = glGetUniformLocation(program, "ourColor");

  while (!glfwWindowShouldClose(window)) {
    if (inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = glfwGetTime();
    float greenValue = (std::sin(timeValue) / 2.0) + 0.5f;

    for (auto &t : triangles) {
      t.bind();
      t.bindShaderProgram();
      glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}