// SPDX-License-Identifier: MIT

#pragma once

#include <vector>

#include <glad/gl.h>

#include "utils/shaderProgram.h"
#include "utils/vertexArray.h"
#include "utils/vertexBuffer.h"
#include "models/particleGpu.h"
#include "rendering/iRenderObject.h"

namespace monsterCatVisualizerOpenGL::rendering
{

class ParticleRenderObject final : public IRenderObject
{
public:
    static constexpr char uniformTime[] = "uTime";
    static constexpr char uniformViewportRatio[] = "uViewportRatio";

    // Construct and destroy only on a thread with a current OpenGL context.
    ParticleRenderObject();

    void initialize() override;
    void synchronize() override;
    void render(const RenderFrameContext& frame) override;

    // Render-thread-only MVP upload; later replaced by published snapshots.
    void setParticles(const std::vector<models::ParticleGpu>& particles);

private:
    utils::ShaderProgram m_shaderProgram;
    utils::VertexBuffer m_vertexBuffer;
    utils::VertexArray m_vertexArray;
    GLsizei m_particleCount{};
    bool m_initialized{};
};

} // namespace monsterCatVisualizerOpenGL::rendering
