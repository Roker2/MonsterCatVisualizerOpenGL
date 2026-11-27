// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>

#include "utils/themeColorState.h"
#include "models/cloudGpu.h"
#include "rendering/containerRenderObject.h"

namespace monsterCatVisualizerOpenGL::rendering
{

class CloudRenderObject final : public ContainerRenderObject<
    monsterCatVisualizer::models::Cloud, models::CloudGpu>
{
public:
    using ColorRGB = monsterCatVisualizer::primitives::ColorRGB<>;
    using ThemeColorStatePtr = std::shared_ptr<monsterCatVisualizer::utils::ThemeColorState>;

    static constexpr char uniformTime[] = "uTime";
    static constexpr char uniformViewportRatio[] = "uViewportRatio";
    static constexpr char uniformThemeColor[] = "uThemeColor";

    /** Construct and destroy only on a thread with a current OpenGL context. */
    CloudRenderObject(ThemeColorStatePtr themeColorState,
        float elementsDensity,
        StoragePtr storagePtr);

    ThemeColorStatePtr getThemeColorState() const;

    void render(const RenderFrameContext& frame) override;

private:
    std::vector<std::string> getUniformNames() override;
    std::vector<utils::VertexAttribute> getVertexAttributes() override;

    ThemeColorStatePtr m_themeColorState;
};

} // namespace monsterCatVisualizerOpenGL::rendering
