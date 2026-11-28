# Copyright (c) 2026 Dzmitry Minko
# SPDX-License-Identifier: MIT

add_executable(MonsterCatVisualizerApp main.cpp)

target_link_libraries(MonsterCatVisualizerApp
    PRIVATE MonsterCatVisualizerOpenGL
)
