cmake_minimum_required(VERSION 4.3.3)

project(opengl-intro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

find_package(OpenGL REQUIRED)

add_subdirectory(external/glfw)

add_executable(${PROJECT_NAME}
  src/glad.c
  src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  OpenGL::GL
)
