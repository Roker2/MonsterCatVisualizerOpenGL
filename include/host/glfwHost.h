// SPDX-License-Identifier: MIT

#pragma once

#include <atomic>
#include <exception>
#include <memory>
#include <thread>

#include "utils/animationClock.h"
#include "utils/viewportState.h"
#include "animation/animationOrchestrator.h"
#include "animation/animationHandlerOrchestrator.h"

struct GLFWwindow;

namespace monsterCatVisualizerOpenGL::host
{

// Owns the GLFW window and a dedicated OpenGL rendering thread.
// Construct, run and destroy on the main thread.
class GlfwHost
{
public:
    using TaskScheduler = monsterCatVisualizer::utils::TaskScheduler;
    using TaskSchedulerPtr = std::shared_ptr<TaskScheduler>;
    using AnimationClock = monsterCatVisualizer::utils::AnimationClock;
    using AnimationClockPtr = std::shared_ptr<AnimationClock>;
    using ViewportState = monsterCatVisualizer::utils::ViewportState;
    using ViewportSize = ViewportState::ViewportSize;
    using ViewportStatePtr = std::shared_ptr<ViewportState>;

    using AnimationOrchestrator = monsterCatVisualizer::animation::AnimationOrchestrator;
    using AnimationOrchestratorPtr = std::unique_ptr<AnimationOrchestrator>;
    using AnimationHandlerOrchestrator = animation::AnimationHandlerOrchestrator;
    using AnimationHandlerOrchestratorPtr = std::unique_ptr<AnimationHandlerOrchestrator>;

    GlfwHost() = default;
    ~GlfwHost();

    GlfwHost(const GlfwHost&) = delete;
    GlfwHost& operator=(const GlfwHost&) = delete;

    int run();

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    void initializeLogic();
    void deinitializeLogic();

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

    TaskSchedulerPtr m_taskScheduler;
    AnimationClockPtr m_animationClock;
    ViewportStatePtr m_viewportState;

    AnimationOrchestratorPtr m_animationOrchestrator;
    AnimationHandlerOrchestratorPtr m_animationHandlerOrchestrator;
};

} // namespace monsterCatVisualizerOpenGL::host
