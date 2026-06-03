cmake_minimum_required(VERSION 4.3.3)

project(opengl-intro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/**/*.cpp")

include_directories(include)

find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  OpenGL::GL
)
