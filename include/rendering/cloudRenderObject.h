// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>

#include "utils/themeColorState.h"
#include "models/cloudGpu.h"
#include "rendering/baseRenderObject.h"
#include "rendering/renderContainerHandler.h"
#include "rendering/renderVisualHandler.h"

namespace monsterCatVisualizerOpenGL::rendering
{

class CloudRenderObject final : public BaseRenderObject
{
public:
    using ColorRGB = monsterCatVisualizer::primitives::ColorRGB<>;
    using DataModel = monsterCatVisualizer::models::Cloud;
    using DataModelGpu = models::CloudGpu;
    using ContainerHandler = RenderContainerHandler<
        DataModel, DataModelGpu>;
    using Storage = ContainerHandler::Storage;
    using StoragePtr = ContainerHandler::StoragePtr;
    using VisualHandler = RenderVisualHandler<DataModelGpu>;
    using ThemeColorStatePtr = std::shared_ptr<monsterCatVisualizer::utils::ThemeColorState>;

    static constexpr char uniformTime[] = "uTime";
    static constexpr char uniformViewportRatio[] = "uViewportRatio";
    static constexpr char uniformThemeColor[] = "uThemeColor";

    /** Construct and destroy only on a thread with a current OpenGL context. */
    CloudRenderObject(ThemeColorStatePtr themeColorState,
        StoragePtr storagePtr,
        float elementsDensity);

    ThemeColorStatePtr getThemeColorState() const;

    void synchronize() override;
    void clear() override;
    void render(const RenderFrameContext& frame) override;

private:
    void initializeInternal(std::size_t desiredCount) override;

    ThemeColorStatePtr m_themeColorState;
    ContainerHandler m_containerHandler;
    VisualHandler m_visualHandler;
};

} // namespace monsterCatVisualizerOpenGL::rendering
