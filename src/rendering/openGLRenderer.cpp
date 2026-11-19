// SPDX-License-Identifier: MIT

#include "rendering/openGLRenderer.h"

#include <glad/gl.h>

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "rendering/particleRenderObject.h"

namespace
{

using namespace monsterCatVisualizerOpenGL;

// Temporary deterministic data to validate VBO/VAO/shader integration.
std::vector<models::ParticleGpu> makeDemoParticles()
{
    std::vector<models::ParticleGpu> result;
    result.reserve(15 * 9);

    for (int y = 0; y < 9; ++y)
    {
        for (int x = 0; x < 15; ++x)
        {
            models::ParticleGpu particle{};
            particle.startPosition[0] = (static_cast<float>(x) + 0.5f) / 15.0f;
            particle.startPosition[1] = (static_cast<float>(y) + 0.5f) / 9.0f;
            particle.direction[0] = 1.0f;
            particle.direction[1] = 0.0f;
            particle.speed = ((x + y) % 7 == 0) ? 0.008f : 0.0f;
            particle.startTime = 0.0f;
            result.push_back(particle);
        }
    }
    return result;
}

} // namespace

namespace monsterCatVisualizerOpenGL::rendering
{

void OpenGLRenderer::initialize()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(1.0f);

    auto particles = std::make_unique<ParticleRenderObject>();
    ParticleRenderObject* demo = particles.get();
    addRenderObject(std::move(particles));
    demo->setParticles(makeDemoParticles()); // TODO: Replace with AnimationHandler snapshots.
}

void OpenGLRenderer::resize(std::int32_t viewportWidth, std::int32_t viewportHeight)
{
    glViewport(0, 0, viewportWidth, viewportHeight);
}

void OpenGLRenderer::render(const RenderFrameContext& frame)
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (const auto& object : m_renderObjects)
    {
        object->synchronize();
        object->render(frame);
    }
}

void OpenGLRenderer::addRenderObject(std::unique_ptr<IRenderObject> renderObject)
{
    if (!renderObject)
        throw std::invalid_argument("OpenGLRenderer: empty render object");

    renderObject->initialize();
    m_renderObjects.push_back(std::move(renderObject));
}

} // namespace monsterCatVisualizerOpenGL::rendering
