// SPDX-License-Identifier: MIT

#include "animation/animationHandlerOrchestrator.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace monsterCatVisualizerOpenGL::animation
{

AnimationHandlerOrchestrator::AnimationHandlerOrchestrator(TaskSchedulerPtr taskScheduler,
    AnimationClockPtr animationClock,
    ViewportStatePtr viewportState)
    : m_taskScheduler(std::move(taskScheduler)),
    m_animationClock(std::move(animationClock)),
    m_viewportState(std::move(viewportState))
{
    if (!m_taskScheduler)
        throw std::runtime_error("AnimationHandlerOrchestrator: empty task scheduler");
    if (!m_animationClock)
        throw std::runtime_error("AnimationHandlerOrchestrator: empty animation clock");
    if (!m_viewportState)
        throw std::runtime_error("AnimationHandlerOrchestrator: empty viewport state");
}

AnimationHandlerOrchestrator::~AnimationHandlerOrchestrator()
{
    removeAllAnimationHandlers();
}

const AnimationHandlerOrchestrator::Storage& AnimationHandlerOrchestrator::getStorage() const
{
    return m_animationHandlers;
}

void AnimationHandlerOrchestrator::addAnimationHandler(AnimationHandlerPtr animationHandler,
    double updateIntervalSeconds)
{
    if (!animationHandler)
        return;

    const auto it = std::find_if(m_animationHandlers.begin(), m_animationHandlers.end(),
        [animationHandler](const auto& descriptor) { return descriptor.animationHandler == animationHandler; });

    if (it != m_animationHandlers.end())
        throw std::runtime_error(
            "AnimationHandlerOrchestrator: adding the same animationHandler twice is not allowed");

    const auto viewportSize = m_viewportState->getSize();
    animationHandler->initialize(viewportSize.width, viewportSize.height);

    const auto taskId = m_taskScheduler->addTask(
        [animationHandler, animationClock = m_animationClock]()
        {
            if (animationClock->isPaused())
                return;
            animationHandler->handleUpdate();
        }, updateIntervalSeconds);

    m_animationHandlers.emplace_back(AnimationHandlerDescriptor{std::move(animationHandler), taskId});
}

void AnimationHandlerOrchestrator::removeAnimationHandler(AnimationHandlerPtr animationHandler)
{
    const auto it = std::find_if(m_animationHandlers.begin(), m_animationHandlers.end(),
        [animationHandler](const auto& descriptor) { return descriptor.animationHandler == animationHandler; });

    if (it != m_animationHandlers.end())
    {
        m_taskScheduler->removeTask(it->taskId);
        m_animationHandlers.erase(it);
    }
}

void AnimationHandlerOrchestrator::removeAnimationHandler(monsterCatVisualizer::utils::TaskScheduler::TaskId taskId)
{
    const auto it = std::find_if(m_animationHandlers.begin(), m_animationHandlers.end(),
        [taskId](const auto& descriptor) { return descriptor.taskId == taskId; });

    if (it != m_animationHandlers.end())
    {
        m_taskScheduler->removeTask(it->taskId);
        m_animationHandlers.erase(it);
    }
}

void AnimationHandlerOrchestrator::removeAllAnimationHandlers()
{
    for (const auto& [_, taskId] : m_animationHandlers)
    {
        m_taskScheduler->removeTask(taskId);
    }
    m_animationHandlers.clear();
}

} // namespace monsterCatVisualizerOpenGL::animation
