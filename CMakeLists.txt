# Copyright (c) 2026 Dzmitry Minko
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.14)

project(MonsterCatVisualizerOpenGL)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  MonsterCatVisualizerBase
  GIT_REPOSITORY https://github.com/Roker2/MonsterCatVisualizerBase.git
)
FetchContent_MakeAvailable(MonsterCatVisualizerBase)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.5.1
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(MonsterCatVisualizerOpenGL ${SOURCES})

target_include_directories(MonsterCatVisualizerOpenGL PRIVATE include)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

target_link_libraries(MonsterCatVisualizerOpenGL PRIVATE MonsterCatVisualizerBase glad glfw OpenGL::GL Threads::Threads)
# Embed GLSL sources into a generated C++ header.
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${SHADER_DIR}/cloud.vert"
    "${SHADER_DIR}/cloud.frag"
    "${SHADER_DIR}/line.vert"
    "${SHADER_DIR}/line.frag"
    "${SHADER_DIR}/particle.vert"
    "${SHADER_DIR}/particle.frag"
)

file(READ "${SHADER_DIR}/cloud.vert" CLOUD_VERT)
file(READ "${SHADER_DIR}/cloud.frag" CLOUD_FRAG)
file(READ "${SHADER_DIR}/line.vert" LINE_VERT)
file(READ "${SHADER_DIR}/line.frag" LINE_FRAG)
file(READ "${SHADER_DIR}/particle.vert" PARTICLE_VERT)
file(READ "${SHADER_DIR}/particle.frag" PARTICLE_FRAG)

configure_file(
    "${SHADER_DIR}/shaders.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/shaders.h"
    @ONLY
)

target_include_directories(MonsterCatVisualizerOpenGL PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)
