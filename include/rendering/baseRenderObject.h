// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>

#include "rendering/renderFrameContext.h"

namespace monsterCatVisualizerOpenGL::rendering
{

/**
 * All methods run on the render thread with a current OpenGL context.
 */
class BaseRenderObject
{
public:
    BaseRenderObject(float elementsDensity);
    virtual ~BaseRenderObject() = default;

    bool isInitialized() const noexcept;

    void initialize(std::int32_t viewportWidth,
        std::int32_t viewportHeight);
    void initialize(std::size_t desiredCount);

    virtual void synchronize() = 0;

    virtual void clear() = 0;

    virtual void render(const RenderFrameContext& frame) = 0;

    virtual std::size_t calculateDesiredCount(std::int32_t viewportWidth,
        std::int32_t viewportHeight) const;

protected:
    virtual void initializeInternal(std::size_t desiredCount) = 0;

private:
    float m_elementsDensity{};
    bool m_initialized{};
};

} // namespace monsterCatVisualizerOpenGL::rendering
