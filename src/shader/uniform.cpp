#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <filesystem>
#include <optional>

#include "gl_utils.h"
#include "shader.h"

void uniforms(GLFWwindow *window,
              std::optional<InputProcessor> inputProcessor) {
  float vertexData[] = {
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

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // returns the uniform location WITHIN this program. It is not a unique/global
  // value. It is a "program-local location" WITHIN THE SPECIFIED PROGRAM

  auto shaderDir = std::filesystem::path(PROJECT_ROOT) / "src" / "shader";
  Shader shader((shaderDir / "vertex.glsl").c_str(),
                (shaderDir / "fragment.glsl").c_str());

  float offset = 0;
  while (!glfwWindowShouldClose(window)) {
    if (inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = glfwGetTime();
    float greenValue = (std::sin(timeValue) / 2.0) + 0.5f;

    glBindVertexArray(VAO);
    shader.use();
    offset += 0.0005f;
    shader.setUniformFloat("offset", offset);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}