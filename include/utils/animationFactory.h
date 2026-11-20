// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <tuple>

#include "animation/cloudsAnimator.h"
// #include "animation/cloudsAnimationHandler.h"
#include "animation/linesAnimator.h"
// #include "animation/linesAnimationHandler.h"
#include "animation/particlesAnimator.h"
#include "animation/particleAnimationHandler.h"

namespace monsterCatVisualizerOpenGL::utils
{

struct AnimationFactory
{
    template <typename T>
    using MinMax = monsterCatVisualizer::primitives::MinMax<T>;

    using ColorRGB = monsterCatVisualizer::primitives::ColorRGB<>;
    using ColorRGBA = monsterCatVisualizer::primitives::ColorRGBA<>;

    template <typename Desc>
    struct AnimationFactoryDescriptor
    {
        using Animator = typename Desc::Animator;
        using AnimationHandler = typename Desc::AnimationHandler;

        using DataModel = typename AnimationHandler::DataModel;
        using DataModelGpu = typename AnimationHandler::DataModelGpu;

        using Storage = typename AnimationHandler::Storage;
        using StoragePtr = typename AnimationHandler::StoragePtr;
    };

    struct CloudsDescriptor
    {
        using Animator = monsterCatVisualizer::animation::CloudsAnimator;
        using AnimationHandler = void;

        static constexpr auto source = Animator::DataModel::GenerationDescriptor
        {
            20.0f, // elementsDensity
            MinMax<float>{0.001f, 0.01f}, // speed
            MinMax<float>{0.01f, 0.1f}, // radius
            MinMax<ColorRGBA> // color
            {
                ColorRGBA{168u, 168u, 168u, 0.4f},
                ColorRGBA{240u, 240u, 240u, 0.9f}
            },
            0.6f // themeColorProbability
        };

        static constexpr auto defaultAnimatorArgs = std::make_tuple(
            ColorRGB{180u, 180u, 180u});
    };

    struct LinesDescriptor
    {
        using Animator = monsterCatVisualizer::animation::LinesAnimator;
        using AnimationHandler = void;

        static constexpr auto source = Animator::DataModel::GenerationDescriptor
        {
            100.0f, // elementsDensity
            MinMax<float>{0.01f, 0.1f}, // speed
            MinMax<float>{0.001f, 0.005f}, // length
            0.01f, // lineWidth
            MinMax<ColorRGBA> // color
            {
                ColorRGBA{168u, 168u, 168u, 0.4f},
                ColorRGBA{240u, 240u, 240u, 0.9f}
            }
        };

        static constexpr auto defaultAnimatorArgs = std::make_tuple();
    };

    struct ParticlesDescriptor
    {
        using Animator = monsterCatVisualizer::animation::ParticlesAnimator;
        using AnimationHandler = monsterCatVisualizerOpenGL::animation::ParticleAnimationHandler;

        static constexpr auto source = Animator::DataModel::GenerationDescriptor
        {
            1000.0f, // elementsDensity
            MinMax<float>{0.01f, 0.1f}, // speed
            MinMax<float>{0.001f, 0.005f}, // radius
            MinMax<ColorRGBA> // color
            {
                ColorRGBA{168u, 168u, 168u, 0.4f},
                ColorRGBA{240u, 240u, 240u, 0.9f}
            }
        };

        static constexpr auto defaultAnimatorArgs = std::make_tuple();
    };

    template <typename Desc, typename Source>
    static auto createDefault(const Source& source = Desc::source)
    {
        return std::apply(create<Desc, Source, decltype(Desc::defaultAnimatorArgs)>,
            Desc::defaultAnimatorArgs);
    }

    template <typename Desc, typename Source, typename... Args>
    static auto create(const Source& source = Desc::source, Args&&... animatorArgs)
    {
        using FactoryDescriptor = AnimationFactoryDescriptor<Desc>;
        auto storage = std::make_shared<typename FactoryDescriptor::Storage>();

        auto animator = std::make_shared<Desc::Animator>(
            std::forward<Args>(animatorArgs)...,
            storage, source);
        auto animationHandler = std::make_shared<Desc::AnimationHandler>(
            storage, source.elementsDensity);

        return std::make_pair(animator, animationHandler);
    }
};

} // namespace monsterCatVisualizerOpenGL::utils
