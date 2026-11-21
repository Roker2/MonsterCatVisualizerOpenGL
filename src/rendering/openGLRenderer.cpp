// SPDX-License-Identifier: MIT

#include "rendering/openGLRenderer.h"

#include <glad/gl.h>

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "rendering/particleRenderObject.h"

namespace monsterCatVisualizerOpenGL::rendering
{

void OpenGLRenderer::initialize()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void OpenGLRenderer::resize(std::int32_t viewportWidth, std::int32_t viewportHeight)
{
    glViewport(0, 0, viewportWidth, viewportHeight);
}

void OpenGLRenderer::render(const RenderFrameContext& frame)
{
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& object : m_renderObjects)
    {
        if (!object->isInitialized())
            object->initialize(frame.viewportWidth, frame.viewportHeight);

        object->synchronize();
        object->render(frame);
    }
}

void OpenGLRenderer::addRenderObject(std::unique_ptr<BaseRenderObject>&& renderObject)
{
    if (!renderObject)
        throw std::invalid_argument("OpenGLRenderer: empty render object");

    m_renderObjects.push_back(std::move(renderObject));
}

void OpenGLRenderer::removeAllRenderObjects()
{
    m_renderObjects.clear();
}

} // namespace monsterCatVisualizerOpenGL::rendering
