cmake_minimum_required(VERSION 3.16)

project(template)


set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/GLAD)
add_subdirectory(libs/glfw)

include_directories(libs/glm)
include_directories(libs/stb)
include_directories(libs/ImGui)
include_directories(libs/ImGui/backends)

add_subdirectory(Source/)

