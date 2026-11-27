// SPDX-License-Identifier: MIT

#include "rendering/particleRenderObject.h"

#include <cstddef>

#include "shaders.h"
#include "utils/mathUtils.h"

namespace monsterCatVisualizerOpenGL::rendering
{

ParticleRenderObject::ParticleRenderObject(float elementsDensity, StoragePtr storagePtr)
    : ContainerRenderObject(shaders::particleVertex, shaders::particleFragment, elementsDensity, storagePtr)
{
}

void ParticleRenderObject::render(const RenderFrameContext& frame)
{
    if (!isInitialized())
        return;

    const float minRadius = monsterCatVisualizer::utils::SpatialMathUtils::convertSSToNSS(
        monsterCatVisualizer::models::Particle::minRadiusPx,
        frame.viewportWidth, frame.viewportHeight);

    m_visualHandler.prepareRendering(
        VisualHandler::UniformPair<GLfloat>{uniformTime, static_cast<GLfloat>(frame.timeSeconds)},
        VisualHandler::UniformPair<GLfloat>{uniformViewportRatio, frame.viewportRatio},
        VisualHandler::UniformPair<GLfloat>{uniformMinRadius, minRadius});

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_visualHandler.getRenderedCount());
}

std::vector<std::string> ParticleRenderObject::getUniformNames()
{
    return std::vector<std::string>
    {
        uniformTime, uniformViewportRatio, uniformMinRadius
    };
}

std::vector<utils::VertexAttribute> ParticleRenderObject::getVertexAttributes()
{
    constexpr GLsizei stride = static_cast<GLsizei>(sizeof(DataModelGpu));
    return std::vector<utils::VertexAttribute>
    {
        {
            0, 2, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, startPosition), 1
        },
        {
            1, 2, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, direction), 1
        },
        {
            2, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, speed), 1
        },
        {
            3, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, startTime), 1
        },
        {
            4, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, lifetime), 1
        },
        {
            5, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, fadeIn), 1
        },
        {
            6, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, fadeOut), 1
        },
        {
            7, 1, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, radius), 1
        },
        {
            8, 4, GL_FLOAT, GL_FALSE, stride, offsetof(DataModelGpu, colorRGBA), 1
        }
    };
}

} // namespace monsterCatVisualizerOpenGL::rendering
