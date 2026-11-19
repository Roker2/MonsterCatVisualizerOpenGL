// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

namespace monsterCatVisualizerOpenGL::rendering
{

// Immutable parameters shared by all render objects in one frame.
struct RenderFrameContext
{
    double timeSeconds{};
    std::int32_t viewportWidth{};
    std::int32_t viewportHeight{};
    float viewportRatio{};
};

} // namespace monsterCatVisualizerOpenGL::rendering
