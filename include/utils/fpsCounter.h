// SPDX-License-Identifier: MIT

#pragma once

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace monsterCatVisualizerOpenGL::utils
{

class FpsCounter
{
    using Clock = std::chrono::steady_clock;
    
    static constexpr double outputInterval = 1.0;
    static constexpr double msConversion = 1000.0;

public:
    void frameRendered()
    {
        ++m_frameCount;

        const auto now = Clock::now();
        const double elapsedSeconds = std::chrono::duration<double>(
            now - m_intervalStart).count();
        if (elapsedSeconds < outputInterval)
            return;

        const double fps = static_cast<double>(m_frameCount) / elapsedSeconds;
        const double averageFrameMs = elapsedSeconds * msConversion / m_frameCount;

        std::ostringstream output;
        output << std::fixed << std::setprecision(1)
               << "FPS: " << fps
               << " | Frame: " << averageFrameMs << " ms";
        std::cout << output.str() << '\n';

        m_intervalStart = now;
        m_frameCount = 0;
    }

    void reset()
    {
        m_intervalStart = Clock::now();
        m_frameCount = 0;
    }

private:
    Clock::time_point m_intervalStart{Clock::now()};
    std::size_t m_frameCount{};
};

} // namespace monsterCatVisualizerOpenGL::utils
