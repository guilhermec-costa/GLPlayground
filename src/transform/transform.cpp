#include "transform.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "shader.h"
#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <stb_image.h>

std::ostream &operator<<(std::ostream &os, const Vec3 &v) {
  os << "x: " << v.x << ", y: " << v.y << ", z: " << v.z;
  return os;
}

static void processInput(GLFWwindow *window);

void transformations(GLFWwindow *window) {
  auto staticPath = std::filesystem::path(PROJECT_ROOT) / "static";
  auto woodTexPath = staticPath/"wood.jpg";

  Texture2D woodTex(woodTexPath.c_str(), GL_RGB, GL_RGB, false);
  woodTex.setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  woodTex.setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
  woodTex.setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  woodTex.setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  woodTex.bindToTexUnit(0);

  auto happyfaceTexPath = staticPath/"happyface.png";
  Texture2D happyTex(happyfaceTexPath.c_str(),  GL_RGB, GL_RGBA, true);
  happyTex.setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  happyTex.setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
  happyTex.setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  happyTex.setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  happyTex.bindToTexUnit(1); 

  float vertexData[] = {
      // positions        // uv
      0.5f,  0.5f,  0.0f,  1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f,  0.0f, 1.0f, // top left
  };

  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  unsigned int VBO, VAO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  auto shaderDir = std::filesystem::path(PROJECT_ROOT) / "src" / "transform" / "shaders";
  Shader shader((shaderDir / "vertex.glsl").c_str(),
                (shaderDir / "fragment.glsl").c_str());

  shader.use();
  // sets the texture unit with the bound texture
  shader.setUniformInt("texture1", 0);
  shader.setUniformInt("texture2", 1);


  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(90.0f),glm::vec3(0.0, 0.0, 1.0));
  model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    woodTex.bindToTexUnit(GL_TEXTURE0);
    happyTex.bindToTexUnit(GL_TEXTURE1);

    shader.use();

    shader.setUniformMatrix4fv("transform", glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

static void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
};