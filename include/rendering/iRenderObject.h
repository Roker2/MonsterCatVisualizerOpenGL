// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

#include "rendering/renderFrameContext.h"

namespace monsterCatVisualizerOpenGL::rendering
{

/**
 * All methods run on the render thread with a current OpenGL context.
 */
class IRenderObject
{
public:
    virtual ~IRenderObject() = default;

    virtual bool isInitialized() const noexcept = 0;
    virtual void initialize(std::int32_t viewportWidth,
        std::int32_t viewportHeight) = 0;

    virtual void synchronize() = 0;
    virtual void clear() = 0;
    virtual void render(const RenderFrameContext& frame) = 0;
};

} // namespace monsterCatVisualizerOpenGL::rendering
