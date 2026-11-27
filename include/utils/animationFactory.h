// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <memory>
#include <tuple>
#include <utility>

#include "animation/cloudsAnimator.h"
#include "animation/linesAnimator.h"
#include "animation/musicBarsAnimator.h"
#include "animation/particlesAnimator.h"
#include "rendering/cloudRenderObject.h"
#include "rendering/lineRenderObject.h"
#include "rendering/musicBarRenderObject.h"
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
    using Vector2 = monsterCatVisualizer::primitives::Vector2<>;

    using IAnimator = monsterCatVisualizer::animation::IAnimator;
    using IAnimatorPtr = std::shared_ptr<IAnimator>;
    using IRenderObject = rendering::IRenderObject;
    using IRenderObjectPtr = std::unique_ptr<IRenderObject>;

    using AnimatorCreationFunc = std::function<IAnimatorPtr()>;
    using RenderObjectCreationFunc = std::function<IRenderObjectPtr()>;

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
        using RenderObject = monsterCatVisualizerOpenGL::rendering::CloudRenderObject;

        static constexpr auto source = Animator::DataModel::GenerationDescriptor
        {
            {
                13.0f, // elementsDensity
                MinMax<float>{0.0005f, 0.005f}, // speed
                MinMax<double>{150.0, 360.0}, // lifetimeSeconds
                MinMax<double>{2.0, 4.0}, // fadeInSeconds
                MinMax<double>{2.5, 5.0}, // fadeOutSeconds
            },

            MinMax<float>{0.11f, 0.17f}, // radius
            MinMax<ColorRGBA> // color
            {
                ColorRGBA{170u, 150u, 180u, 0.25f},
                ColorRGBA{240u, 195u, 250u, 0.65f}
            },
            0.6f // themeColorProbability
        };

        static constexpr double updateIntervalSeconds = 0.5;
    };

    struct LinesDescriptor
    {
        using Animator = monsterCatVisualizer::animation::LinesAnimator;
        using RenderObject = monsterCatVisualizerOpenGL::rendering::LineRenderObject;

        static constexpr auto source = Animator::DataModel::GenerationDescriptor
        {
            {
                12.0f, // elementsDensity
                MinMax<float>{0.001f, 0.005f}, // speed
                MinMax<double>{120.0, 210.0}, // lifetimeSeconds
                MinMax<double>{2.5, 4.0}, // fadeInSeconds
                MinMax<double>{1.5, 4.5}, // fadeOutSeconds
            },

            MinMax<float>{0.03f, 0.08f}, // length
            0.0025f, // lineWidth
            MinMax<ColorRGBA> // color
            {
                ColorRGBA{200u, 195u, 210u, 0.6f},
                ColorRGBA{235u, 220u, 250u, 0.85f}
            }
        };

        static constexpr double updateIntervalSeconds = 0.4;
    };

    struct ParticlesDescriptor
    {
        using Animator = monsterCatVisualizer::animation::ParticlesAnimator;
        using RenderObject = monsterCatVisualizerOpenGL::rendering::ParticleRenderObject;

        static constexpr auto source = Animator::DataModel::GenerationDescriptor
        {
            {
                3000.0f, // elementsDensity
                MinMax<float>{0.00025f, 0.0025f}, // speed
                MinMax<double>{90.0, 150.0}, // lifetimeSeconds
                MinMax<double>{1.0, 2.0}, // fadeInSeconds
                MinMax<double>{1.5, 2.5}, // fadeOutSeconds
            },

            MinMax<float>{0.0005f, 0.0035f}, // radius
            MinMax<ColorRGBA> // color
            {
                ColorRGBA{190u, 185u, 200u, 0.4f},
                ColorRGBA{240u, 225u, 250u, 0.9f}
            }
        };

        static constexpr double updateIntervalSeconds = 0.1;
    };

    struct MusicBarsDescriptor
    {
        using Animator = monsterCatVisualizer::animation::MusicBarsAnimator;
        using RenderObject = monsterCatVisualizerOpenGL::rendering::MusicBarRenderObject;

        static constexpr auto sourceMusicBarsState = monsterCatVisualizer::utils::MusicBarsState::StateDescriptor
        {
            ColorRGBA{190u, 50u, 80u, 1.0f}, // color
            Vector2{}, // panelCenterOffset
            Vector2{0.5f, 0.5f}, // panelSize
            0.7f // elementWidthScale
        };

        static constexpr double updateIntervalSeconds = 0.09;
    };

    /**
     * Creation functions must be invoked exactly once.
     * @return two functors: create animator, create render object
     */
    template <typename Desc, typename AnimatorTuple, typename RenderObjectTuple>
    static auto create(AnimatorTuple&& animatorTuple, RenderObjectTuple&& renderObjectTuple)
    {
        using FactoryDescriptor = AnimationFactoryDescriptor<Desc>;
        auto storage = std::make_shared<typename FactoryDescriptor::Storage>();

        AnimatorCreationFunc animatorCreationFunc =
            [animatorTuple = std::forward<AnimatorTuple>(animatorTuple), storage]() mutable
        {
            return std::apply([&](auto&&... items)
            {
                return std::make_shared<Desc::Animator>(
                    std::move(items)..., std::move(storage));
            }, std::move(animatorTuple));
        };

        RenderObjectCreationFunc renderObjectCreationFunc =
            [renderObjectTuple = std::forward<RenderObjectTuple>(renderObjectTuple), storage]() mutable
        {
            return std::apply([&](auto&&... items)
            {
                return std::make_unique<Desc::RenderObject>(
                    std::move(items)..., std::move(storage));
            }, std::move(renderObjectTuple));
        };

        return std::make_tuple(animatorCreationFunc, renderObjectCreationFunc);
    }
};

} // namespace monsterCatVisualizerOpenGL::utils
