// SPDX-License-Identifier: MIT

#include "rendering/baseRenderObject.h"

#include <cmath>
#include <stdexcept>

#include "utils/defines.h"
#include "utils/mathUtils.h"

namespace monsterCatVisualizerOpenGL::rendering
{

BaseRenderObject::BaseRenderObject(float elementsDensity)
    : m_elementsDensity(elementsDensity)
{
    const bool elementsDensityValid = std::isfinite(elementsDensity) &&
        elementsDensity > monsterCatVisualizer::utils::elementsDensityMin;
    if (!elementsDensityValid)
        throw std::runtime_error("BaseRenderObject: invalid elementsDensity");
}

bool BaseRenderObject::isInitialized() const noexcept
{
    return m_initialized;
}

void BaseRenderObject::initialize(std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    const std::size_t desiredCount = calculateDesiredCount(viewportWidth, viewportHeight);
    initialize(desiredCount);
}

void BaseRenderObject::initialize(std::size_t desiredCount)
{
    if (m_initialized)
        throw std::logic_error("BaseRenderObject: already initialized");

    initializeInternal(desiredCount);

    m_initialized = true;
}

std::size_t BaseRenderObject::calculateDesiredCount(std::int32_t viewportWidth,
    std::int32_t viewportHeight) const
{
    return monsterCatVisualizer::utils::SpatialMathUtils::calculateDesiredCount(
        m_elementsDensity, viewportWidth, viewportHeight);
}

} // namespace monsterCatVisualizerOpenGL::rendering
