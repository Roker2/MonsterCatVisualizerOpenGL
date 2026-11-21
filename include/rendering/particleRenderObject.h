// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

#include "models/particleGpu.h"
#include "rendering/baseRenderObject.h"
#include "rendering/renderContainerHandler.h"
#include "rendering/renderVisualHandler.h"

namespace monsterCatVisualizerOpenGL::rendering
{

class ParticleRenderObject final : public BaseRenderObject
{
public:
    using DataModel = monsterCatVisualizer::models::Particle;
    using DataModelGpu = models::ParticleGpu;
    using ContainerHandler = RenderContainerHandler<
        DataModel, DataModelGpu>;
    using Storage = ContainerHandler::Storage;
    using StoragePtr = ContainerHandler::StoragePtr;
    using VisualHandler = RenderVisualHandler<DataModelGpu>;

    static constexpr char uniformTime[] = "uTime";
    static constexpr char uniformViewportRatio[] = "uViewportRatio";

    /** Construct and destroy only on a thread with a current OpenGL context. */
    ParticleRenderObject(StoragePtr storagePtr, float elementsDensity);

    using BaseRenderObject::initialize;

    void synchronize() override;
    void clear() override;
    void render(const RenderFrameContext& frame) override;

private:
    void initializeInternal(std::size_t desiredCount) override;

    ContainerHandler m_containerHandler;
    VisualHandler m_visualHandler;
};

} // namespace monsterCatVisualizerOpenGL::rendering
