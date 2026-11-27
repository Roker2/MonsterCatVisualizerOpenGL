// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>

#include "utils/musicBarsState.h"
#include "models/simpleContainer.h"
#include "rendering/iRenderObject.h"
#include "rendering/renderVisualHandler.h"

namespace monsterCatVisualizerOpenGL::rendering
{

class MusicBarRenderObject final : public IRenderObject
{
public:
    using ColorRGBA = monsterCatVisualizer::primitives::ColorRGBA<>;
    using Vector2 = monsterCatVisualizer::primitives::Vector2<>;
    using MusicBarsStatePtr = std::shared_ptr<monsterCatVisualizer::utils::MusicBarsState>;
    
    using DataModel = float;
    using DataModelGpu = float;

    using VisualHandler = RenderVisualHandler<DataModelGpu>;
    using Storage = monsterCatVisualizer::models::SimpleContainer<DataModel>;
    using StoragePtr = std::shared_ptr<Storage>;

    static constexpr char uniformCount[] = "uCount";
    static constexpr char uniformColor[] = "uColor";
    static constexpr char uniformPanelCenterOffset[] = "uPanelCenterOffset";
    static constexpr char uniformPanelSize[] = "uPanelSize";
    static constexpr char uniformElementWidthScale[] = "uElementWidthScale";

    /** Construct and destroy only on a thread with a current OpenGL context. */
    MusicBarRenderObject(MusicBarsStatePtr musicBarsState,
        StoragePtr storagePtr);

    MusicBarsStatePtr getMusicBarsState() const;

    bool isInitialized() const noexcept override;
    void initialize(std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;

    void synchronize() override;
    void clear() override;
    void render(const RenderFrameContext& frame) override;

private:
    MusicBarsStatePtr m_musicBarsState;
    StoragePtr m_storagePtr;
    std::vector<DataModelGpu> m_storageGpu;
    VisualHandler m_visualHandler;
    bool m_initialized{};
};

} // namespace monsterCatVisualizerOpenGL::rendering
