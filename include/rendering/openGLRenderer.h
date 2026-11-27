// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <vector>

#include "rendering/iRenderObject.h"

namespace monsterCatVisualizerOpenGL::rendering
{

/**
 * All methods run on the render thread with an active OpenGL context.
 */
class OpenGLRenderer
{
public:
    OpenGLRenderer() = default;

    OpenGLRenderer(const OpenGLRenderer&) = delete;
    OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;

    void initialize();
    void resize(std::int32_t viewportWidth, std::int32_t viewportHeight);
    void render(const RenderFrameContext& frame);

    /** Takes ownership and initializes the object on the render thread. */
    void addRenderObject(std::unique_ptr<IRenderObject>&& renderObject);

    void removeAllRenderObjects();

private:
    std::vector<std::unique_ptr<IRenderObject>> m_renderObjects;
};

} // namespace monsterCatVisualizerOpenGL::rendering
