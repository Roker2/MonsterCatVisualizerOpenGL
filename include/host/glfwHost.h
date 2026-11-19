// SPDX-License-Identifier: MIT

#pragma once

#include <atomic>
#include <exception>
#include <memory>
#include <thread>

#include "utils/animationClock.h"
#include "utils/viewportState.h"

struct GLFWwindow;

namespace monsterCatVisualizerOpenGL::host
{

// Owns the GLFW window and a dedicated OpenGL rendering thread.
// Construct, run and destroy on the main thread.
class GlfwHost
{
public:
    using AnimationClock = monsterCatVisualizer::utils::AnimationClock;
    using AnimationClockPtr = std::shared_ptr<AnimationClock>;
    using ViewportState = monsterCatVisualizer::utils::ViewportState;
    using ViewportSize = ViewportState::ViewportSize;
    using ViewportStatePtr = std::shared_ptr<ViewportState>;

    GlfwHost() = default;
    ~GlfwHost();

    GlfwHost(const GlfwHost&) = delete;
    GlfwHost& operator=(const GlfwHost&) = delete;

    int run();

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    void initializeLogic();

    void initializeGLFW();
    void deinitializeGLFW();

    void initializeRendering();
    void deinitializeRendering();

    void setFramebufferSize(int width, int height);
    void renderLoop() noexcept;

    GLFWwindow* m_window{};
    bool m_glfwInitialized{};
    std::thread m_renderThread;
    std::atomic<bool> m_stop{false};
    std::exception_ptr m_renderError;

    AnimationClockPtr m_animationClock;
    ViewportStatePtr m_viewportState;
};

} // namespace monsterCatVisualizerOpenGL::host
