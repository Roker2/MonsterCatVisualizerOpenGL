// SPDX-License-Identifier: MIT

#include "rendering/cloudRenderObject.h"

#include <cstddef>
#include <stdexcept>

#include "shaders.h"

namespace monsterCatVisualizerOpenGL::rendering
{

CloudRenderObject::CloudRenderObject(ThemeColorStatePtr themeColorState,
    StoragePtr storagePtr,
    float elementsDensity)
    : BaseRenderObject(elementsDensity),
    m_themeColorState(std::move(themeColorState)),
    m_containerHandler(storagePtr),
    m_visualHandler(shaders::cloudVertex, shaders::cloudFragment)
{
    if (!m_themeColorState)
        throw std::runtime_error("CloudRenderObject: empty themeColorState");
}

CloudRenderObject::ThemeColorStatePtr CloudRenderObject::getThemeColorState() const
{
    return m_themeColorState;
}

void CloudRenderObject::synchronize()
{
    if (!isInitialized())
        throw std::logic_error("CloudRenderObject: not initialized");

    if (!m_containerHandler.synchronize())
        return;

    m_visualHandler.synchronize(m_containerHandler.getStorageGpu(),
        m_containerHandler.getLastPatch().modifiedRanges,
        m_containerHandler.needsFullUpload());
}

void CloudRenderObject::clear()
{
    m_containerHandler.clear();
    m_visualHandler.clear();
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

void CloudRenderObject::initializeInternal(std::size_t desiredCount)
{
    m_containerHandler.initialize(desiredCount);

    const std::vector<std::string> uniformNames
    {
        uniformTime, uniformViewportRatio, uniformThemeColor
    };

    constexpr GLsizei stride = static_cast<GLsizei>(sizeof(DataModelGpu));
    const std::vector<utils::VertexAttribute> attributes
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

    m_visualHandler.initialize(desiredCount,
        uniformNames,
        attributes);
    m_visualHandler.synchronize(m_containerHandler.getStorageGpu());
}

} // namespace monsterCatVisualizerOpenGL::rendering
