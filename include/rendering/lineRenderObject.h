// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

#include "models/lineGpu.h"
#include "rendering/baseRenderObject.h"
#include "rendering/renderContainerHandler.h"
#include "rendering/renderVisualHandler.h"

namespace monsterCatVisualizerOpenGL::rendering
{

class LineRenderObject final : public BaseRenderObject
{
public:
    using DataModel = monsterCatVisualizer::models::Line;
    using DataModelGpu = models::LineGpu;
    using ContainerHandler = RenderContainerHandler<
        DataModel, DataModelGpu>;
    using Storage = ContainerHandler::Storage;
    using StoragePtr = ContainerHandler::StoragePtr;
    using VisualHandler = RenderVisualHandler<DataModelGpu>;

    static constexpr char uniformTime[] = "uTime";
    static constexpr char uniformViewportRatio[] = "uViewportRatio";

    /** Construct and destroy only on a thread with a current OpenGL context. */
    LineRenderObject(StoragePtr storagePtr, float elementsDensity);

    void synchronize() override;
    void clear() override;
    void render(const RenderFrameContext& frame) override;

private:
    void initializeInternal(std::size_t desiredCount) override;

    ContainerHandler m_containerHandler;
    VisualHandler m_visualHandler;
};

} // namespace monsterCatVisualizerOpenGL::rendering
