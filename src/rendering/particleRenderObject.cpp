// SPDX-License-Identifier: MIT

#include "rendering/particleRenderObject.h"

#include <cstddef>
#include <limits>
#include <stdexcept>

#include "shaders.h"

namespace monsterCatVisualizerOpenGL::rendering
{

ParticleRenderObject::ParticleRenderObject()
    : m_shaderProgram(shaders::particleVertex, shaders::particleFragment)
{
}

void ParticleRenderObject::initialize()
{
    if (m_initialized)
        throw std::logic_error("ParticleRenderObject: already initialized");

    m_shaderProgram.createUniforms({uniformTime, uniformViewportRatio});

    using ParticleGpu = models::ParticleGpu;
    constexpr GLsizei stride = static_cast<GLsizei>(sizeof(ParticleGpu));

    m_vertexArray.addAttribute(m_vertexBuffer,
        {
            0, 2, GL_FLOAT, GL_FALSE, stride, offsetof(ParticleGpu, startPosition)
        });
    m_vertexArray.addAttribute(m_vertexBuffer,
        {
            1, 2, GL_FLOAT, GL_FALSE, stride, offsetof(ParticleGpu, direction)
        });
    m_vertexArray.addAttribute(m_vertexBuffer,
        {
            2, 1, GL_FLOAT, GL_FALSE, stride, offsetof(ParticleGpu, speed)
        });
    m_vertexArray.addAttribute(m_vertexBuffer,
        {
            3, 1, GL_FLOAT, GL_FALSE, stride, offsetof(ParticleGpu, startTime)
        });

    utils::VertexArray::unbind();
    utils::VertexBuffer::unbind();
    m_initialized = true;
}

void ParticleRenderObject::synchronize()
{
    // TODO: Consume a published snapshot and update m_vertexBuffer only when changed.
}

void ParticleRenderObject::render(const RenderFrameContext& frame)
{
    if (!m_initialized || m_particleCount == 0)
        return;

    m_shaderProgram.bind();
    m_shaderProgram.setUniform(uniformTime, static_cast<GLfloat>(frame.timeSeconds));
    m_shaderProgram.setUniform(uniformViewportRatio, frame.viewportRatio);
    m_vertexArray.bind();
    glDrawArrays(GL_POINTS, 0, m_particleCount);
}

void ParticleRenderObject::setParticles(const std::vector<models::ParticleGpu>& particles)
{
    if (!m_initialized)
        throw std::logic_error("ParticleRenderObject: not initialized");

    constexpr auto maxParticles = static_cast<std::size_t>(std::numeric_limits<GLsizeiptr>::max()) /
        sizeof(models::ParticleGpu);
    if (particles.size() > maxParticles)
        throw std::length_error("ParticleRenderObject: too many particles");

    const auto bytes = static_cast<GLsizeiptr>(particles.size() * sizeof(models::ParticleGpu));
    m_vertexBuffer.allocate(bytes, GL_DYNAMIC_DRAW, particles.empty() ? nullptr : particles.data());
    m_particleCount = static_cast<GLsizei>(particles.size());
}

} // namespace monsterCatVisualizerOpenGL::rendering
