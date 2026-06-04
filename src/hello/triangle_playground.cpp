#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "gl_utils.h"
#include <optional>

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

void trianglePlayground(GLFWwindow *window, std::optional<InputProcessor> inputProcessor) {

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

  const char *vss = vertexShaderSource.c_str();
  glShaderSource(vertexShader, 1, &vss, NULL);
  glCompileShader(vertexShader);
  if (checkShaderCompilation(vertexShader) == -1) {
    return;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fss = fragmentShaderSource.c_str();
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

  float vertices[] = {
      // first triangle
      0.5f, 0.70f, 0.0f, 
      0.35, 0.4f, 0.0f, 
      0.65f, 0.4f, 0.0f,

      //second triangle
      -0.5f, 0.70f, 0.0f, 
      -0.35, 0.4f, 0.0f, 
      -0.65f, 0.4f, 0.0f,

      // third triangle
      -0.5f, -0.35f, 0.0f, 
      -0.35, -0.7f, 0.0f, 
      -0.65f, -0.7f, 0.0f,

      // fourth triangle
      0.5f, -0.35f, 0.0f, 
      0.35, -0.7f, 0.0f, 
      0.65f, -0.7f, 0.0f,
  };

  unsigned int VAO, VBO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if(inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }


    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 12);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);

  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}
