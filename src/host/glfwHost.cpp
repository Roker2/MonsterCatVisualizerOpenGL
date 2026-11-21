// SPDX-License-Identifier: MIT

#include "host/glfwHost.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <stdexcept>

#include "utils/mathUtils.h"
#include "rendering/openGLRenderer.h"

namespace
{

constexpr double pollEventsTimeoutSeconds = 0.05;
constexpr double sleepTimeoutSeconds = 0.025;

void errorCallback(int error, const char* description)
{
    std::cerr << "GLFW error: " << error << ": " << description << std::endl;
}

template <typename Desc, typename AnimationOrchestrator, typename RenderObjectCreationStorage>
void createDefault(AnimationOrchestrator& animationOrchestrator, RenderObjectCreationStorage& renderObjectCreationStorage)
{
    using AnimationFactory = monsterCatVisualizerOpenGL::utils::AnimationFactory;

    auto [animatorCreationFunc, renderObjectCreationFunc] =
        AnimationFactory::createDefault<Desc>();

    animationOrchestrator.addAnimator(animatorCreationFunc(), Desc::updateIntervalSeconds);
    renderObjectCreationStorage.emplace_back(std::move(renderObjectCreationFunc));
}

} // namespace

namespace monsterCatVisualizerOpenGL::host
{

GlfwHost::~GlfwHost()
{
    deinitializeAnimation();
    deinitializeRendering();
    deinitializeGLFW();
    deinitializeBasicModels();
}

int GlfwHost::run()
{
    initializeBasicModels();
    initializeGLFW();
    initializeAnimation();
    initializeRendering();

    while (!glfwWindowShouldClose(m_window) && !m_stop.load(std::memory_order_relaxed))
    {
        // Avoid spinning the main thread while renderLoop() draws continuously.
        glfwWaitEventsTimeout(pollEventsTimeoutSeconds);
    }

    deinitializeRendering();
    deinitializeAnimation();

    if (m_renderError)
        std::rethrow_exception(m_renderError);

    return 0;
}

void GlfwHost::initializeBasicModels()
{
    m_taskScheduler = std::make_shared<TaskScheduler>();
    m_animationClock = std::make_shared<AnimationClock>();
    m_viewportState = std::make_shared<ViewportState>();
}

void GlfwHost::deinitializeBasicModels()
{
    m_taskScheduler.reset();
    m_animationClock.reset();
    m_viewportState.reset();
}

void GlfwHost::initializeGLFW()
{
    glfwSetErrorCallback(errorCallback);

    if (glfwInit() != GLFW_TRUE)
        throw std::runtime_error("GlfwHost: glfwInit failed");
    m_glfwInitialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(640, 480, "MonsterCatVisualizer", nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("GlfwHost: glfwCreateWindow failed");

    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, &GlfwHost::keyCallback);
    glfwSetFramebufferSizeCallback(m_window, &GlfwHost::framebufferSizeCallback);

    // Window and event operations remain on the main thread.
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    setFramebufferSize(width, height);
}

void GlfwHost::deinitializeGLFW()
{
    if (m_window)
        glfwDestroyWindow(m_window);

    if (m_glfwInitialized)
        glfwTerminate();
}

void GlfwHost::initializeAnimation()
{
    m_animationOrchestrator = std::make_unique<AnimationOrchestrator>(
        m_taskScheduler, m_animationClock, m_viewportState);

    createDefault<utils::AnimationFactory::ParticlesDescriptor>(
        *m_animationOrchestrator, m_renderObjectCreationStorage);
}

void GlfwHost::deinitializeAnimation()
{
    m_animationOrchestrator.reset();
}

void GlfwHost::initializeRendering()
{
    // The new context is not current on the main thread.
    // Only renderLoop() makes it current and calls OpenGL.
    m_renderThread = std::thread(&GlfwHost::renderLoop, this);
}

void GlfwHost::deinitializeRendering()
{
    m_stop.store(true, std::memory_order_relaxed);

    if (m_renderThread.joinable())
        m_renderThread.join();
}

void GlfwHost::keyCallback(GLFWwindow* window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void GlfwHost::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    auto* host = static_cast<GlfwHost*>(glfwGetWindowUserPointer(window));
    if (host)
        host->setFramebufferSize(width, height);
}

void GlfwHost::setFramebufferSize(int width, int height)
{
    m_viewportState->setSize({ width, height });
}

void GlfwHost::renderLoop() noexcept
{
    try
    {
        glfwMakeContextCurrent(m_window);
        if (glfwGetCurrentContext() != m_window)
            throw std::runtime_error("GlfwHost: could not make OpenGL context current");

        if (gladLoadGL(glfwGetProcAddress) == 0)
            throw std::runtime_error("GlfwHost: could not initialize GLAD");

        glfwSwapInterval(1);

        // Renderer and its future GL resources are destroyed before the context is released.
        {
            rendering::OpenGLRenderer renderer;
            renderer.initialize();

            // currently m_renderObjectCreationStorage is designed to be read only once
            // without any synchronization on the rendering thread
            for (const auto& renderObjectCreationFunc : m_renderObjectCreationStorage)
            {
                renderer.addRenderObject(renderObjectCreationFunc());
            }
            m_renderObjectCreationStorage.clear();

            ViewportSize previousSize{-1, -1};

            while (!m_stop.load(std::memory_order_relaxed))
            {
                ViewportSize currentSize = m_viewportState->getSize();
                const auto timeSample = m_animationClock->getTimeSample();

                if (!monsterCatVisualizer::utils::SpatialMathUtils::viewportSizeValid(
                    currentSize.width, currentSize.height))
                {
                    std::this_thread::sleep_for(std::chrono::duration<double>(sleepTimeoutSeconds));
                    continue;
                }

                const bool viewportChanged = currentSize.width != previousSize.width ||
                    currentSize.height != previousSize.height;
                if (viewportChanged)
                {
                    renderer.resize(currentSize.width, currentSize.height);
                    previousSize = currentSize;
                }

                const bool shouldRender = !timeSample.paused || viewportChanged;
                if (shouldRender)
                {
                    const rendering::RenderFrameContext frame{
                        timeSample.timeSeconds,
                        currentSize.width,
                        currentSize.height,
                        monsterCatVisualizer::utils::SpatialMathUtils::viewportToRatio(
                            currentSize.width, currentSize.height)
                    };
                    renderer.render(frame);
                    glfwSwapBuffers(m_window);
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::duration<double>(sleepTimeoutSeconds));
                }
            }
        }
    }
    catch (...)
    {
        m_renderError = std::current_exception();
        m_stop.store(true, std::memory_order_relaxed);
        // Wake the main thread if it is waiting for events.
        glfwPostEmptyEvent();
    }

    glfwMakeContextCurrent(nullptr);
}

} // namespace monsterCatVisualizerOpenGL::host
