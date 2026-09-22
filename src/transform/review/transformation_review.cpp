#include "gl_utils.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "shader.h"
#include "texture_utils.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <filesystem>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void transformations_review(GLFWwindow *window, std::optional<InputProcessor> inputProcessor) {
  // texture config
  auto asset_dir = std::filesystem::path(PROJECT_ROOT) / "static";
  auto woodTex = asset_dir / "wood.jpg";
  auto happyfaceTex = asset_dir / "happyface.png";

  uint woodTexId, happyfaceTexId;
  load_texture(&woodTexId, woodTex.c_str(), GL_RGB, GL_RGB);
  load_texture(&happyfaceTexId, happyfaceTex.c_str(), GL_RGB, GL_RGBA);

  float vertexData[] = {
      // positions         // uv
      0.5f,  0.5f,  0.0f,  1.0f, 0.0f, // top right (0)
      0.5f,  -0.5f, 0.0f,  1.0f, 1.0f, // bottom right (1)
      -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, // bottom left (2)
      -0.5f, 0.5f,  0.0f,  0.0f, 0.0f, // top left (3)
  };

  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  // buffer config
  unsigned int VBO, VAO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindVertexArray(0);

  // shader loading
  auto shaderDir = std::filesystem::path(PROJECT_ROOT) / "src"/ "transform" / "review";
  Shader shader((shaderDir / "vertex.glsl").c_str(),
                (shaderDir / "frag.glsl").c_str());

  shader.use();
  shader.setUniformInt("TEXTURE1", 0);
  shader.setUniformInt("TEXTURE2", 1);


  glm::mat4 trans_mat;

  while(!glfwWindowShouldClose(window)) {
    if(inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    trans_mat = glm::mat4(1.0f);
    // trans_mat = glm::translate(trans_mat, glm::vec3(0.1f, -0.4f, 0.0f));
    trans_mat = glm::rotate(trans_mat, (float)glfwGetTime() * 0.9f, glm::vec3(0.0, 0.0, 1.0));
    trans_mat = glm::scale(trans_mat, glm::vec3(0.5, 0.5, 0.5));

    bind_to_active_texture(0, woodTexId);
    bind_to_active_texture(1, happyfaceTexId);
    shader.setUniformMat4fv("transform", glm::value_ptr(trans_mat));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}