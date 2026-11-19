// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <vector>

#include "utils/animationClock.h"
#include "utils/viewportState.h"
#include "utils/taskScheduler.h"
#include "animation/baseAnimationHandler.h"

namespace monsterCatVisualizerOpenGL::animation
{

class AnimationHandlerOrchestrator
{
public:
    struct AnimationHandlerDescriptor
    {
        using AnimationHandlerPtr = std::shared_ptr<BaseAnimationHandler>;

        AnimationHandlerPtr animationHandler{};
        monsterCatVisualizer::utils::TaskScheduler::TaskId taskId{};
    };

    using AnimationHandlerPtr = AnimationHandlerDescriptor::AnimationHandlerPtr;
    using Storage = std::vector<AnimationHandlerDescriptor>;
    using TaskSchedulerPtr = std::shared_ptr<monsterCatVisualizer::utils::TaskScheduler>;
    using AnimationClockPtr = std::shared_ptr<monsterCatVisualizer::utils::AnimationClock>;
    using ViewportStatePtr = std::shared_ptr<monsterCatVisualizer::utils::ViewportState>;

    AnimationHandlerOrchestrator(TaskSchedulerPtr taskScheduler,
        AnimationClockPtr animationClock,
        ViewportStatePtr viewportState);
    ~AnimationHandlerOrchestrator();

    const Storage& getStorage() const;

    /**
     * Add, initialize and schedule an animation handler.
     * ViewportState must contain a valid size before initialization.
     */
    void addAnimationHandler(AnimationHandlerPtr animationHandler, double updateIntervalSeconds);
    void removeAnimationHandler(AnimationHandlerPtr animationHandler);
    void removeAnimationHandler(monsterCatVisualizer::utils::TaskScheduler::TaskId taskId);
    void removeAllAnimationHandlers();

private:
    Storage m_animationHandlers;
    TaskSchedulerPtr m_taskScheduler;
    AnimationClockPtr m_animationClock;
    ViewportStatePtr m_viewportState;
};

} // namespace monsterCatVisualizerOpenGL::animation
