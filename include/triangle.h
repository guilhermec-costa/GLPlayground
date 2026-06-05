#pragma once

#include "glad/glad.h"
#include <cstddef>

class Triangle {

public:
  Triangle(float *vertexData, size_t size, unsigned int program)
      : vertexData(vertexData), size(size) {
    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);
    this->program = program;
  }

  Triangle(Triangle &&other) noexcept
      : vertexData(other.vertexData), size(other.size), VAO(other.VAO),
        VBO(other.VBO), program(other.program)  {
    other.size = 0;
    other.VAO = 0;
    other.VBO = 0;
    other.vertexShader = 0;
    other.fragmentShader = 0;
    other.program = 0;
    other.vertexData = nullptr;
  }

  unsigned int getVAO() { return this->VAO; }

  void draw() {
    this->bind();
    this->bindShaderProgram();
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

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

  void bindShaderProgram() { glUseProgram(this->program); }

  ~Triangle() {
    glDeleteBuffers(1, &this->VBO);
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteProgram(this->program);
  };

private:
  Triangle(Triangle &) = delete;
  Triangle &operator=(const Triangle &) = delete;

private:
  float *vertexData;
  size_t size;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int vertexShader, fragmentShader, program;
};