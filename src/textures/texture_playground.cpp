#include <glad/glad.h>
#include "gl_utils.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <optional>

void texturePlayground(GLFWwindow *window,
                       std::optional<InputProcessor> inputProcessor) {
  float vertexData[] = {
    // position         // color
    0.0f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
  };

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  auto shaderDir = std::filesystem::path(PROJECT_ROOT) / "src" / "textures";
  Shader shader((shaderDir / "vertex.glsl").c_str(),
                (shaderDir / "frag.glsl").c_str());


  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  float offset = 0;
  while (!glfwWindowShouldClose(window)) {

    if (inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }
    shader.use();
    offset += 0.00005;
    shader.setUniformFloat("offset", offset);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}