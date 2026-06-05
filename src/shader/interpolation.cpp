#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <string>

#include "gl_utils.h"

static std::string t1VSS = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

void main() {
  gl_Position = vec4(aPos, 1.0);
  vertexColor = aColor; 
}
)";

static std::string t1FSS = R"(
#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

void main() {
  FragColor = vec4(vertexColor, 1.0); 
}
)";

void interpolation(GLFWwindow *window,
                   std::optional<InputProcessor> inputProcessor) {

  float vertices[] = {
      // positions         // colors
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right vertex
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left vertex
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top vertex
  };

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
      /* color attribute offset*/ (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  auto program = createShaderProgram(t1VSS, t1FSS);

  // returns the uniform location WITHIN this program. It is not a unique/global
  // value. It is a "program-local location" WITHIN THE SPECIFIED PROGRAM
  int vertexColorLocation = glGetUniformLocation(program, "ourColor");

  while (!glfwWindowShouldClose(window)) {
    if (inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}