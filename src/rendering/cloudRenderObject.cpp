// SPDX-License-Identifier: MIT

#include "rendering/cloudRenderObject.h"

#include <cstddef>
#include <stdexcept>

#include "shaders.h"

namespace monsterCatVisualizerOpenGL::rendering
{

CloudRenderObject::CloudRenderObject(ThemeColorStatePtr themeColorState,
    float elementsDensity,
    StoragePtr storagePtr)
    : ContainerRenderObject(shaders::cloudVertex, shaders::cloudFragment, elementsDensity, storagePtr),
    m_themeColorState(std::move(themeColorState))
{
    if (!m_themeColorState)
        throw std::runtime_error("CloudRenderObject: empty themeColorState");
}

CloudRenderObject::ThemeColorStatePtr CloudRenderObject::getThemeColorState() const
{
    return m_themeColorState;
}

void CloudRenderObject::render(const RenderFrameContext& frame)
{
    if (!isInitialized())
        return;

    m_visualHandler.prepareRendering(
        VisualHandler::UniformPair<GLfloat>{uniformTime, static_cast<GLfloat>(frame.timeSeconds)},
        VisualHandler::UniformPair<GLfloat>{uniformViewportRatio, frame.viewportRatio},
        VisualHandler::UniformPair<ColorRGB>{uniformThemeColor, m_themeColorState->getColor()});

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_visualHandler.getRenderedCount());
}

std::vector<std::string> CloudRenderObject::getUniformNames()
{
    return std::vector<std::string>
    {
        uniformTime, uniformViewportRatio, uniformThemeColor
    };
}

std::vector<utils::VertexAttribute> CloudRenderObject::getVertexAttributes()
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
            7, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, radius), 1
        },
        {
            8, 4, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, colorRGBA), 1
        },
        {
            9, 1, GL_INT, GL_FALSE, stride, offsetof(DataModelGpu, isThemeColored), 1, utils::VertexAttributeKind::Integer
        }
    };
}

} // namespace monsterCatVisualizerOpenGL::rendering
