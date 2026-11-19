// SPDX-License-Identifier: MIT

#pragma once

#include "rendering/renderFrameContext.h"

namespace monsterCatVisualizerOpenGL::rendering
{

// All methods run on the render thread with a current OpenGL context.
class IRenderObject
{
public:
    virtual ~IRenderObject() = default;

    virtual void initialize() = 0;
    virtual void synchronize() = 0;
    virtual void render(const RenderFrameContext& frame) = 0;
};

} // namespace monsterCatVisualizerOpenGL::rendering
