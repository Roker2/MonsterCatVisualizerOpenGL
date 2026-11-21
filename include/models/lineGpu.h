// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>
#include <glad/gl.h>

#include "models/line.h"

namespace monsterCatVisualizerOpenGL::models
{

struct LineGpu
{
    static LineGpu convert(
        const monsterCatVisualizer::models::Line& line)
    {
        return LineGpu
        {
            {
                line.trajectory.startPosition.x,
                line.trajectory.startPosition.y
            },
            {
                line.trajectory.direction.x,
                line.trajectory.direction.y
            },
            line.trajectory.speed,
            static_cast<GLfloat>(line.trajectory.startTimeSeconds),

            line.geometry.length,
            line.geometry.lineWidth,
            {
                line.visual.color.r / 255.0f,
                line.visual.color.g / 255.0f,
                line.visual.color.b / 255.0f,
                line.visual.color.a
            }
        };
    }

    GLfloat startPosition[2]{};
    GLfloat direction[2]{};
    GLfloat speed{};
    GLfloat startTime{};

    GLfloat length{};
    GLfloat lineWidth{};
    GLfloat colorRGBA[4]{};
};

static_assert(std::is_standard_layout_v<LineGpu>);
static_assert(std::is_trivially_copyable_v<LineGpu>);
static_assert(sizeof(GLfloat) == 4);

} // namespace monsterCatVisualizerOpenGL::models
