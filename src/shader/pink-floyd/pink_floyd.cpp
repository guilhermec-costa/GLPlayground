#include "gl_utils.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <optional>

void pinkFloyd(GLFWwindow *window,std::optional<InputProcessor> inputProcessor) {

  unsigned int mainTriangleVBO, secondaryTriangleVBO, innerTriangleVBO;
  unsigned int mainTriangleVAO, secondaryTriangleVAO, innerTriangleVAO;

  float mainTriangleVertexData[] = {
    // position
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
  };

  glGenBuffers(1, &mainTriangleVBO);
  glGenVertexArrays(1, &mainTriangleVAO);

  glBindVertexArray(mainTriangleVAO);

  glBindBuffer(GL_ARRAY_BUFFER, mainTriangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mainTriangleVertexData), mainTriangleVertexData, GL_STATIC_DRAW);


  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  auto pinkFloydDir = std::filesystem::path(PROJECT_ROOT) / "src" / "shader" / "pink-floyd";

  Shader mainTriangleShader((pinkFloydDir/"main_triangle.vs").c_str(),(pinkFloydDir/"main_triangle.fs").c_str());

  glBindVertexArray(0);
  while(!glfwWindowShouldClose(window)) {
    if(inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(mainTriangleVAO);

    mainTriangleShader.use();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}