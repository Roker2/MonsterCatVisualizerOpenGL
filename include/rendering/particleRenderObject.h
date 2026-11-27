// SPDX-License-Identifier: MIT

#pragma once

#include "models/particleGpu.h"
#include "rendering/containerRenderObject.h"

namespace monsterCatVisualizerOpenGL::rendering
{

class ParticleRenderObject final : public ContainerRenderObject<
    monsterCatVisualizer::models::Particle, models::ParticleGpu>
{
public:
    static constexpr char uniformTime[] = "uTime";
    static constexpr char uniformViewportRatio[] = "uViewportRatio";
    static constexpr char uniformMinRadius[] = "uMinRadius";

    /** Construct and destroy only on a thread with a current OpenGL context. */
    ParticleRenderObject(float elementsDensity, StoragePtr storagePtr);

    void render(const RenderFrameContext& frame) override;

private:
    std::vector<std::string> getUniformNames() override;
    std::vector<utils::VertexAttribute> getVertexAttributes() override;
};

} // namespace monsterCatVisualizerOpenGL::rendering
