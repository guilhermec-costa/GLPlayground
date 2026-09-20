#include "gl_utils.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <optional>
#include <stb_image.h>

void load_texture(uint* texId, const char* texpath, int internalformat, int pixelformat) {
  glGenTextures(1, texId);
  glBindTexture(GL_TEXTURE_2D, *texId);

  int w, h, nrch;
  unsigned char *texData = stbi_load(texpath, &w, &h, &nrch, 0);
  if(texData) {
    glTexImage2D(
      GL_TEXTURE_2D, 
      0, 
      internalformat, 
      w, h, 0, 
      pixelformat, 
      GL_UNSIGNED_BYTE, 
      texData
    );
    stbi_image_free(texData);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void bind_to_active_texture(short texIndex, uint texId) {
  glActiveTexture(GL_TEXTURE0 + texIndex);
  glBindTexture(GL_TEXTURE_2D, texId);
}

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

  while(!glfwWindowShouldClose(window)) {
    if(inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    bind_to_active_texture(0, woodTexId);
    bind_to_active_texture(1, happyfaceTexId);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}