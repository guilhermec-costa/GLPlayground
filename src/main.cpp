#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

/*
  GLSL
  vec4: vec.x, vec.y, vec.z and vec.w (perspective division)
  gl_Position: predefined output variable. It is the output of the vertex shader
*/
std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

std::string fragmentShaderSource = R"(# 
#version 330 core
out vec4 FragColor;

void main(){
  FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)";

void framebufferResizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int checkShaderCompilation(unsigned int shaderId);
int checkProgramLinkingStatus(unsigned int programId);

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

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

  const char *vss = vertexShaderSource.c_str();
  glShaderSource(vertexShader, 1, &vss, NULL);
  glCompileShader(vertexShader);

  if (checkShaderCompilation(vertexShader) == -1) {
    return -1;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  const char *fss = fragmentShaderSource.c_str();
  glShaderSource(fragmentShader, 1, &fss, NULL);
  glCompileShader(fragmentShader);

  if (checkShaderCompilation(fragmentShader) == -1) {
    return -1;
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  if (checkProgramLinkingStatus(shaderProgram) == -1) {
    return -1;
  }

  glUseProgram(shaderProgram);
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
    processInput(window);

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

  glfwTerminate();
  return 0;
}

void framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  std::cout << "resizing window to w: " << width << " h: " << height << "\n";
  glViewport(0, 0, width, height);
};

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int checkShaderCompilation(unsigned int shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;

    return -1;
  }

  return 0;
}

int checkProgramLinkingStatus(unsigned int programId) {
  int success;
  char infoLog[512];
  glGetProgramiv(programId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    return -1;
  }
  return 0;
}