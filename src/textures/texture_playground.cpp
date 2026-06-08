#include <glad/glad.h>
#include "gl_utils.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <optional>
#include <stb_image.h>

void texturePlayground(GLFWwindow *window,
                       std::optional<InputProcessor> inputProcessor) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture); 

  // texture type, axis, wrap mode 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  // minifying = more distance from the texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  // magnifying = less distance from the texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  auto staticPath = std::filesystem::path(PROJECT_ROOT) / "static";
  auto texturePath = staticPath/"wood.jpg";

  std::cout << "Loading texture from path " << texturePath.string() << "\n";
  int width, height, nrChannels;
  unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);


  if(data) {
    std::cout << "Texture loaded from " << texturePath.string() << "\n";
    // generate a texture image to the current bound texture object
    glTexImage2D(GL_TEXTURE_2D, /* mipmap level */ 0, 
      /* destination color format*/ GL_RGB, width, height, 0, 
      /* source color format*/ GL_RGB, 
      /* source data format */ GL_UNSIGNED_BYTE, 
      data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "failed to load texture\n";
  }
  stbi_image_free(data);

  float vertexData[] = {
    // positions        // colors           // uv
    0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top left 
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
  };

  unsigned int VBO, VAO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  auto shaderDir = std::filesystem::path(PROJECT_ROOT) / "src" / "textures";
  Shader shader((shaderDir / "vertex.glsl").c_str(),
                (shaderDir / "frag.glsl").c_str());
  while (!glfwWindowShouldClose(window)) {
    if (inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}