// SPDX-License-Identifier: MIT

#pragma once

#include "models/particle.h"
#include "models/particleGpu.h"
#include "animation/containerAnimationHandler.h"

namespace monsterCatVisualizerOpenGL::animation
{

class ParticleAnimationHandler : public ContainerAnimationHandler<
    monsterCatVisualizer::models::Particle,
    models::ParticleGpu>
{
public:
    ParticleAnimationHandler(StoragePtr storagePtr, float elementsDensity);
};

} // namespace monsterCatVisualizerOpenGL::animation
