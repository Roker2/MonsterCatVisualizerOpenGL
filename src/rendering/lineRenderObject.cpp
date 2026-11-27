// SPDX-License-Identifier: MIT

#include "rendering/lineRenderObject.h"

#include <cstddef>

#include "shaders.h"

namespace monsterCatVisualizerOpenGL::rendering
{

LineRenderObject::LineRenderObject(float elementsDensity, StoragePtr storagePtr)
    : ContainerRenderObject(shaders::lineVertex, shaders::lineFragment, elementsDensity, storagePtr)
{
}

void LineRenderObject::render(const RenderFrameContext& frame)
{
    if (!isInitialized())
        return;

    m_visualHandler.prepareRendering(
        VisualHandler::UniformPair<GLfloat>{uniformTime, static_cast<GLfloat>(frame.timeSeconds)},
        VisualHandler::UniformPair<GLfloat>{uniformViewportRatio, frame.viewportRatio});

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_visualHandler.getRenderedCount());
}

std::vector<std::string> LineRenderObject::getUniformNames()
{
    return std::vector<std::string>
    {
        uniformTime, uniformViewportRatio
    };
}

std::vector<utils::VertexAttribute> LineRenderObject::getVertexAttributes()
{
    constexpr GLsizei stride = static_cast<GLsizei>(sizeof(DataModelGpu));
    return std::vector<utils::VertexAttribute>
    {
        {
            0, 2, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, startPosition), 1
        },
        {
            1, 2, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, direction), 1
        },
        {
            2, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, speed), 1
        },
        {
            3, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, startTime), 1
        },
        {
            4, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, lifetime), 1
        },
        {
            5, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, fadeIn), 1
        },
        {
            6, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, fadeOut), 1
        },
        {
            7, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, length), 1
        },
        {
            8, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, lineWidth), 1
        },
        {
            9, 4, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, colorRGBA), 1
        }
    };
}

} // namespace monsterCatVisualizerOpenGL::rendering
