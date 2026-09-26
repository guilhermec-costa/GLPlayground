#include "gl_utils.h"
#include "shader.h"
#include "texture_utils.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <optional>
#include <filesystem>
#include <random>

static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

bool is_pressed(GLFWwindow* w, int key) {
  if(glfwGetKey(w, key) == GLFW_PRESS) {
    return true;
  }
  return false;
}

glm::vec3 get_rand_norm_vec3(
  std::mt19937& gen, 
  std::uniform_real_distribution<float> dist
) {
  glm::vec3 vec = glm::vec3(dist(gen), dist(gen), dist(gen));
  return glm::normalize(vec);
}

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float lastX = 400, lastY = 300;
float yaw = -90.f, pitch = 0.0f;
float fov = 45.0f;
bool firstMouse = true;


void coordinate_system(GLFWwindow* window, std::optional<InputProcessor> inputProcessor) {

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  auto asset_dir = std::filesystem::path(PROJECT_ROOT) / "static";
  auto woodTex = asset_dir / "wood.jpg";
  auto happyfaceTex = asset_dir / "shrek.png";

  uint woodTexId, happyfaceTexId;
  load_texture(&woodTexId, woodTex.c_str(), GL_RGB, GL_RGB);
  load_texture(&happyfaceTexId, happyfaceTex.c_str(), GL_RGB, GL_RGBA);

  float vertexData[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f), 
      glm::vec3( 2.0f,  5.0f, -15.0f), 
      glm::vec3(-1.5f, -2.2f, -2.5f),  
      glm::vec3(-3.8f, -2.0f, -12.3f),  
      glm::vec3( 2.4f, -0.4f, -3.5f),  
      glm::vec3(-1.7f,  3.0f, -7.5f),  
      glm::vec3( 1.3f, -2.0f, -2.5f),  
      glm::vec3( 1.5f,  2.0f, -2.5f), 
      glm::vec3( 1.5f,  0.2f, -1.5f), 
      glm::vec3(-1.3f,  1.0f, -1.5f)  
  };

  glm::vec3 cubeColors[] = {
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist)
  };

  glm::vec3 cubeRotations[] = {
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist),
    get_rand_norm_vec3(gen, dist)
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
  auto shaderDir = std::filesystem::path(PROJECT_ROOT) / "src"/ "coord_systems";
  Shader shader((shaderDir / "vertex.glsl").c_str(),
                (shaderDir / "frag.glsl").c_str());

  shader.use();
  shader.setUniformInt("TEXTURE1", 0);
  shader.setUniformInt("TEXTURE2", 1);


  glm::mat4 model_mat;

  glEnable(GL_DEPTH_TEST);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  while(!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if(inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }
    const float cameraSpeed = 4.0f * deltaTime;
    if(is_pressed(window, GLFW_KEY_W)) {
      cameraPos += cameraSpeed * cameraFront;
    }
    if(is_pressed(window, GLFW_KEY_S)) {
      cameraPos -= cameraSpeed * cameraFront;
    }
    if(is_pressed(window, GLFW_KEY_A)) {
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if(is_pressed(window, GLFW_KEY_D)) {
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if(is_pressed(window, GLFW_KEY_SPACE)) {
      cameraPos.y += cameraSpeed; 
    }
    if(is_pressed(window, GLFW_KEY_LEFT_SHIFT)) {
      cameraPos.y -= cameraSpeed;
    }
    // -------------------------------------------

    glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bind_to_active_texture(0, woodTexId);
    bind_to_active_texture(1, happyfaceTexId);

    shader.use();
    shader.setUniformFloat("time", glm::sin((float)glfwGetTime()) + 1.5f);

    const float radius = 20.0f;
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader.setUniformMat4fv("view", glm::value_ptr(view));

    glm::mat4 proj_mat = glm::mat4(1.0);
    proj_mat = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
    shader.setUniformMat4fv("projection", glm::value_ptr(proj_mat));

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i<10; i++) {
      model_mat = glm::mat4(1.0f);
      model_mat = glm::translate(model_mat, cubePositions[i]);
      float angle = 20.0f * (i + 1);
      shader.setUniformMat4fv("model", glm::value_ptr(model_mat));

      glDrawArrays(GL_TRIANGLES, 0, 36); // performs a draw call
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if(firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
  lastX = xpos;
  lastY = ypos;
  
  const float sensitivity = 0.01f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
    pitch = 89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  cameraFront = glm::normalize(direction);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  fov -= (float)yoffset;
  if(fov < 1.0f) {
    fov = 1.0f;
  }
  if(fov > 45.0f) {
    fov = 45.0f;
  }
}