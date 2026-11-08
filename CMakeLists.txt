# Copyright (c) 2026 Dzmitry Minko
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.13)

project(MonsterCatVisualizerOpenGL)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  MonsterCatVisualizerBase
  GIT_REPOSITORY https://github.com/Roker2/MonsterCatVisualizerBase.git
  EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(MonsterCatVisualizerBase)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.5.1
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(glfw)

add_executable(MonsterCatVisualizerOpenGL src/main.cpp)

find_package(OpenGL REQUIRED)

add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

target_link_libraries(MonsterCatVisualizerOpenGL PRIVATE MonsterCatVisualizerBase glfw OpenGL::GL glad)