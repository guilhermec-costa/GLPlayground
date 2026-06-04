#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl_utils.h"
#include <string>
#include <optional>

/*
  GLSL
  vec4: vec.x, vec.y, vec.z and vec.w (perspective division)
  gl_Position: predefined output variable. It is the output of the vertex shader
*/
static std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

static std::string fragmentShaderSource = R"( 
#version 330 core
out vec4 FragColor;

void main(){
  FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)";

void helloTriangle(GLFWwindow* window, std::optional<InputProcessor> inputProcessor) {
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

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right vertex (0)
      0.5f,  -0.5f, 0.0f, // bottom right (1)
      -0.5f, -0.5f, 0.0f, // bottom left (2)
      -0.5f, 0.5f,  0.0f  // top left (3)
  };

  unsigned int indices[] = {0, 1, 3, 1, 2, 3}; // vertex indices

  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // GL_ARRAY_BUFFER is the buffer type for a vertex buffer object (VBO)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    // frame start
    if(inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 5);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
    // frame end
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);
};