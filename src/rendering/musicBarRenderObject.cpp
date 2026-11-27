// SPDX-License-Identifier: MIT

#include "rendering/musicBarRenderObject.h"

#include <cstddef>
#include <stdexcept>

#include "shaders.h"

namespace monsterCatVisualizerOpenGL::rendering
{

MusicBarRenderObject::MusicBarRenderObject(MusicBarsStatePtr musicBarsState,
    StoragePtr storagePtr)
    : m_musicBarsState(std::move(musicBarsState)),
    m_storagePtr(std::move(storagePtr)),
    m_visualHandler(shaders::musicBarVertex, shaders::musicBarFragment)
{
    if (!m_musicBarsState)
        throw std::runtime_error("MusicBarRenderObject: empty musicBarsState");
    if (!m_storagePtr)
        throw std::runtime_error("CloudRenderObject: empty storage");
}

MusicBarRenderObject::MusicBarsStatePtr MusicBarRenderObject::getMusicBarsState() const
{
    return m_musicBarsState;
}

bool MusicBarRenderObject::isInitialized() const noexcept
{
    return m_initialized;
}

void MusicBarRenderObject::initialize(std::int32_t /* viewportWidth */,
    std::int32_t /* viewportHeight */)
{
    if (m_initialized)
        throw std::logic_error("MusicBarRenderObject: already initialized");

    const std::vector<std::string> uniformNames
    {
        uniformCount,
        uniformColor,
        uniformPanelCenterOffset,
        uniformPanelSize,
        uniformElementWidthScale
    };

    constexpr GLsizei stride = static_cast<GLsizei>(sizeof(DataModelGpu));
    const std::vector<utils::VertexAttribute> attributes
    {
        {
            0, 1, GL_FLOAT, GL_FALSE, stride, 0, 1
        }
    };

    m_storagePtr->getData(m_storageGpu);
    m_visualHandler.initialize(m_storageGpu.size(),
        uniformNames,
        attributes);
    m_visualHandler.synchronize(m_storageGpu);

    m_initialized = true;
}

void MusicBarRenderObject::synchronize()
{
    if (!isInitialized())
        throw std::logic_error("MusicBarRenderObject: not initialized");

    m_storagePtr->getData(m_storageGpu);
    m_visualHandler.synchronize(m_storageGpu);
}

void MusicBarRenderObject::clear()
{
    m_visualHandler.clear();
}

void MusicBarRenderObject::render(const RenderFrameContext& /* frame */)
{
    if (!isInitialized())
        return;

    const auto musicBarsState = m_musicBarsState->get();

    m_visualHandler.prepareRendering(
        VisualHandler::UniformPair<GLint>{uniformCount, m_visualHandler.getRenderedCount()},
        VisualHandler::UniformPair<ColorRGBA>{uniformColor, musicBarsState.color},
        VisualHandler::UniformPair<Vector2>{uniformPanelCenterOffset, musicBarsState.panelCenterOffset},
        VisualHandler::UniformPair<Vector2>{uniformPanelSize, musicBarsState.panelSize},
        VisualHandler::UniformPair<float>{uniformElementWidthScale, musicBarsState.elementWidthScale});

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_visualHandler.getRenderedCount());
}

} // namespace monsterCatVisualizerOpenGL::rendering
