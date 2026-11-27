// SPDX-License-Identifier: MIT

#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "utils/defines.h"
#include "utils/mathUtils.h"
#include "rendering/iRenderObject.h"
#include "rendering/renderContainerHandler.h"
#include "rendering/renderVisualHandler.h"

namespace monsterCatVisualizerOpenGL::rendering
{

/**
 * All methods run on the render thread with a current OpenGL context.
 */
template <typename T, typename U>
class ContainerRenderObject : public IRenderObject
{
public:
    using DataModel = T;
    using DataModelGpu = U;

    using ContainerHandler = RenderContainerHandler<DataModel, DataModelGpu>;
    using VisualHandler = RenderVisualHandler<DataModelGpu>;
    using Storage = typename ContainerHandler::Storage;
    using StoragePtr = typename ContainerHandler::StoragePtr;

    bool isInitialized() const noexcept override
    {
        return m_initialized;
    }

    void initialize(std::int32_t viewportWidth,
        std::int32_t viewportHeight) override
    {
        if (m_initialized)
            throw std::logic_error("ContainerRenderObject: already initialized");

        const std::size_t desiredCount = calculateDesiredCount(viewportWidth, viewportHeight);

        m_containerHandler.initialize(desiredCount);
        m_visualHandler.initialize(desiredCount,
            getUniformNames(),
            getVertexAttributes());
        m_visualHandler.synchronize(m_containerHandler.getStorageGpu());

        m_initialized = true;
    }

    void synchronize() override
    {
        if (!isInitialized())
            throw std::logic_error("ContainerRenderObject: not initialized");

        if (!m_containerHandler.synchronize())
            return;

        m_visualHandler.synchronize(m_containerHandler.getStorageGpu(),
            m_containerHandler.getLastPatch().modifiedRanges,
            m_containerHandler.needsFullUpload());
    }

    void clear() override
    {
        m_containerHandler.clear();
        m_visualHandler.clear();
    }

protected:
    ContainerRenderObject(std::string_view vertexSource,
        std::string_view fragmentSource,
        float elementsDensity,
        StoragePtr storagePtr)
        : m_containerHandler(storagePtr),
        m_visualHandler(vertexSource, fragmentSource),
        m_elementsDensity(elementsDensity)
    {
        const bool elementsDensityValid = std::isfinite(m_elementsDensity) &&
            m_elementsDensity > monsterCatVisualizer::utils::elementsDensityMin;
        if (!elementsDensityValid)
            throw std::runtime_error("ContainerRenderObject: invalid elementsDensity");
    }

    virtual std::vector<std::string> getUniformNames() = 0;
    virtual std::vector<utils::VertexAttribute> getVertexAttributes() = 0;

    std::size_t calculateDesiredCount(std::int32_t viewportWidth,
        std::int32_t viewportHeight) const
    {
        return monsterCatVisualizer::utils::SpatialMathUtils::calculateDesiredCount(
            m_elementsDensity, viewportWidth, viewportHeight);
    }

    ContainerHandler m_containerHandler;
    VisualHandler m_visualHandler;

private:
    float m_elementsDensity{};
    bool m_initialized{};
};

} // namespace monsterCatVisualizerOpenGL::rendering
