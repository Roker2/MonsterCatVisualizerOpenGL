// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

#include "models/lineGpu.h"
#include "rendering/containerRenderObject.h"

namespace monsterCatVisualizerOpenGL::rendering
{

class LineRenderObject final : public ContainerRenderObject<
    monsterCatVisualizer::models::Line, models::LineGpu>
{
public:
    static constexpr char uniformTime[] = "uTime";
    static constexpr char uniformViewportRatio[] = "uViewportRatio";

    /** Construct and destroy only on a thread with a current OpenGL context. */
    LineRenderObject(float elementsDensity, StoragePtr storagePtr);

    void render(const RenderFrameContext& frame) override;

private:
    std::vector<std::string> getUniformNames() override;
    std::vector<utils::VertexAttribute> getVertexAttributes() override;
};

} // namespace monsterCatVisualizerOpenGL::rendering
