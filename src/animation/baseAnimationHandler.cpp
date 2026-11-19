// SPDX-License-Identifier: MIT

#include "animation/baseAnimationHandler.h"

namespace monsterCatVisualizerOpenGL::animation
{

void BaseAnimationHandler::initialize(std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    const std::size_t desiredCount = calculateDesiredCount(viewportWidth, viewportHeight);
    initialize(desiredCount);
}

} // namespace monsterCatVisualizerOpenGL::animation
