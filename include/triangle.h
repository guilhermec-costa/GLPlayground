#pragma once

#include "glad/glad.h"
#include <cstddef>

class Triangle {
private:
  Triangle(Triangle &) = delete;
  Triangle &operator=(const Triangle &) = delete;

public:
  Triangle(float *vertexData, std::size_t size)
      : vertexData(vertexData), size(size) {
    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);
  }

  Triangle(Triangle &&other) noexcept
      : vertexData(other.vertexData), size(other.size), VAO(other.VAO),
        VBO(other.VBO) {
    other.size = 0;
    other.VAO = 0;
    other.VBO = 0;
    other.vertexData = nullptr;
  }

  unsigned int getVAO() { return this->VAO; }

  void make() {
    this->bind();

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->size, this->vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void bind() { glBindVertexArray(this->VAO); }

  ~Triangle() {
    glDeleteBuffers(1, &this->VBO);
    glDeleteVertexArrays(1, &this->VAO);
  };

private:
  float *vertexData;
  size_t size;
  unsigned int VAO;
  unsigned int VBO;
};