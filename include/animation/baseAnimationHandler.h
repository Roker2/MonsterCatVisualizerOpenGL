// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstdint>

namespace monsterCatVisualizerOpenGL::animation
{

class BaseAnimationHandler
{
public:
    virtual ~BaseAnimationHandler() = default;

    virtual void initialize(std::int32_t viewportWidth,
        std::int32_t viewportHeight);
    virtual void initialize(std::size_t desiredCount) = 0;
    virtual void clear() = 0;
    virtual void handleUpdate() = 0;
    virtual std::size_t calculateDesiredCount(std::int32_t viewportWidth,
        std::int32_t viewportHeight) const = 0;
};

} // namespace monsterCatVisualizerOpenGL::animation
