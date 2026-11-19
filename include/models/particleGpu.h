// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>
#include <glad/gl.h>

#include "models/particle.h"

namespace monsterCatVisualizerOpenGL::models
{

struct ParticleGpu
{
    static ParticleGpu convert(
        const monsterCatVisualizer::models::Particle& particle)
    {
        return ParticleGpu
        {
            {
                particle.trajectory.startPosition.x,
                particle.trajectory.startPosition.y
            },
            {
                particle.trajectory.direction.x,
                particle.trajectory.direction.y
            },
            particle.trajectory.speed,
            static_cast<GLfloat>(particle.trajectory.startTimeSeconds),

            particle.geometry.radius,
            {
                particle.visual.color.r / 255.0f,
                particle.visual.color.g / 255.0f,
                particle.visual.color.b / 255.0f,
                particle.visual.color.a
            }
        };
    }

    GLfloat startPosition[2]{};
    GLfloat direction[2]{};
    GLfloat speed{};
    GLfloat startTime{};

    GLfloat radius{};
    GLfloat colorRGBA[4]{};
};

static_assert(std::is_standard_layout_v<ParticleGpu>);
static_assert(std::is_trivially_copyable_v<ParticleGpu>);
static_assert(sizeof(GLfloat) == 4);

} // namespace monsterCatVisualizerOpenGL::models
