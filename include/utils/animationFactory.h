// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <memory>
#include <tuple>

#include "animation/cloudsAnimator.h"
// #include "rendering/cloudRenderObject.h"
#include "animation/linesAnimator.h"
#include "rendering/lineRenderObject.h"
#include "animation/particlesAnimator.h"
#include "rendering/particleRenderObject.h"

namespace monsterCatVisualizerOpenGL::utils
{

/** It could be the rainbow factory */
struct AnimationFactory
{
    template <typename T>
    using MinMax = monsterCatVisualizer::primitives::MinMax<T>;

    using ColorRGB = monsterCatVisualizer::primitives::ColorRGB<>;
    using ColorRGBA = monsterCatVisualizer::primitives::ColorRGBA<>;

    using BaseAnimator = monsterCatVisualizer::animation::BaseAnimator;
    using BaseAnimatorPtr = std::shared_ptr<BaseAnimator>;
    using BaseRenderObject = rendering::BaseRenderObject;
    using BaseRenderObjectPtr = std::unique_ptr<BaseRenderObject>;

    using AnimatorCreationFunc = std::function<BaseAnimatorPtr()>;
    using RenderObjectCreationFunc = std::function<BaseRenderObjectPtr()>;

    template <typename Desc>
    struct AnimationFactoryDescriptor
    {
        using Animator = typename Desc::Animator;
        using RenderObject = typename Desc::RenderObject;

        using DataModel = typename RenderObject::DataModel;
        using DataModelGpu = typename RenderObject::DataModelGpu;

        using Storage = typename RenderObject::Storage;
        using StoragePtr = typename RenderObject::StoragePtr;
    };

    struct CloudsDescriptor
    {
        using Animator = monsterCatVisualizer::animation::CloudsAnimator;
        using RenderObject = void;

        static constexpr auto source = Animator::DataModel::GenerationDescriptor
        {
            20.0f, // elementsDensity
            MinMax<float>{0.0001f, 0.001f}, // speed
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
        static constexpr double updateIntervalSeconds = 0.5;
    };

    struct LinesDescriptor
    {
        using Animator = monsterCatVisualizer::animation::LinesAnimator;
        using RenderObject = monsterCatVisualizerOpenGL::rendering::LineRenderObject;

        static constexpr auto source = Animator::DataModel::GenerationDescriptor
        {
            40.0f, // elementsDensity
            MinMax<float>{0.01f, 0.02f}, // speed
            MinMax<float>{0.01f, 0.05f}, // length
            0.001f, // lineWidth
            MinMax<ColorRGBA> // color
            {
                ColorRGBA{168u, 168u, 168u, 0.4f},
                ColorRGBA{240u, 240u, 240u, 0.9f}
            }
        };

        static constexpr auto defaultAnimatorArgs = std::make_tuple();
        static constexpr double updateIntervalSeconds = 0.1;
    };

    struct ParticlesDescriptor
    {
        using Animator = monsterCatVisualizer::animation::ParticlesAnimator;
        using RenderObject = monsterCatVisualizerOpenGL::rendering::ParticleRenderObject;

        static constexpr auto source = Animator::DataModel::GenerationDescriptor
        {
            500.0f, // elementsDensity
            MinMax<float>{0.001f, 0.01f}, // speed
            MinMax<float>{0.0005f, 0.0025f}, // radius
            MinMax<ColorRGBA> // color
            {
                ColorRGBA{168u, 168u, 168u, 0.4f},
                ColorRGBA{240u, 240u, 240u, 0.9f}
            }
        };

        static constexpr auto defaultAnimatorArgs = std::make_tuple();
        static constexpr double updateIntervalSeconds = 0.1;
    };

    template <typename Desc, typename Source = decltype(Desc::source)>
    static auto createDefault(const Source& source = Desc::source)
    {
        return std::apply(
            [&source](auto&&... args)
            {
                return create<Desc>(source, std::forward<decltype(args)>(args)...);
            },
            Desc::defaultAnimatorArgs);
    }

    template <typename Desc, typename Source, typename... Args>
    static auto create(const Source& source = Desc::source, Args&&... animatorArgs)
    {
        using FactoryDescriptor = AnimationFactoryDescriptor<Desc>;
        auto storage = std::make_shared<typename FactoryDescriptor::Storage>();

        AnimatorCreationFunc animatorCreationFunc =
            [animatorArgs..., storage, source]() mutable
        {
            return std::make_shared<Desc::Animator>(
                std::forward<Args>(animatorArgs)...,
                storage, source);
        };
        RenderObjectCreationFunc renderObjectCreationFunc =
            [storage, elementsDensity = source.elementsDensity]()
        {
            return std::make_unique<Desc::RenderObject>(
                storage, elementsDensity);
        };

        return std::make_tuple(animatorCreationFunc, renderObjectCreationFunc);
    }
};

} // namespace monsterCatVisualizerOpenGL::utils
