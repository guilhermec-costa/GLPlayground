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

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)


add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_definitions(${PROJECT_NAME}
  PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  OpenGL::GL
  glm::glm
)
