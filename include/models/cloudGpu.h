// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>
#include <glad/gl.h>

#include "models/cloud.h"

namespace monsterCatVisualizerOpenGL::models
{

struct CloudGpu
{
    static CloudGpu convert(
        const monsterCatVisualizer::models::Cloud& cloud)
    {
        return CloudGpu
        {
            {
                cloud.trajectory.startPosition.x,
                cloud.trajectory.startPosition.y
            },
            {
                cloud.trajectory.direction.x,
                cloud.trajectory.direction.y
            },
            cloud.trajectory.speed,
            static_cast<GLfloat>(cloud.trajectory.startTimeSeconds),

            cloud.geometry.radius,
            {
                cloud.visual.color.r / 255.0f,
                cloud.visual.color.g / 255.0f,
                cloud.visual.color.b / 255.0f,
                cloud.visual.color.a
            },
            cloud.visual.isThemeColored
        };
    }

    GLfloat startPosition[2]{};
    GLfloat direction[2]{};
    GLfloat speed{};
    GLfloat startTime{};

    GLfloat radius{};
    GLfloat colorRGBA[4]{};
    GLint isThemeColored{};
};

static_assert(std::is_standard_layout_v<CloudGpu>);
static_assert(std::is_trivially_copyable_v<CloudGpu>);
static_assert(sizeof(GLfloat) == 4);

} // namespace monsterCatVisualizerOpenGL::models
