// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <memory>
#include <tuple>

#include "animation/cloudsAnimator.h"
#include "rendering/cloudRenderObject.h"
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
            // MinMax<ColorRGBA> // color
            // {
            //     ColorRGBA{190u, 185u, 200u, 0.4f},
            //     ColorRGBA{240u, 225u, 250u, 0.9f}
            // }
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

    /**
     * Creation functions must be invoked exactly once.
     * @return two functors: create animator, create render object
     */
    template <typename Desc, typename Source = decltype(Desc::source)>
    static auto createDefault(const Source& source = Desc::source)
    {
        using FactoryDescriptor = AnimationFactoryDescriptor<Desc>;
        auto storage = std::make_shared<typename FactoryDescriptor::Storage>();

        AnimatorCreationFunc animatorCreationFunc =
            [storage, source]() mutable
        {
            return std::make_shared<Desc::Animator>(
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

    /**
     * Creation functions must be invoked exactly once.
     * @return two functors: create animator, create render object
     */
    template <typename Desc, typename Source, typename... Args>
    static auto create(const Source& source, Args&&... customArgs)
    {
        using FactoryDescriptor = AnimationFactoryDescriptor<Desc>;
        auto storage = std::make_shared<typename FactoryDescriptor::Storage>();

        AnimatorCreationFunc animatorCreationFunc =
            [customArgs..., storage, source]() mutable
        {
            return std::make_shared<Desc::Animator>(
                std::forward<Args>(customArgs)...,
                storage, source);
        };
        RenderObjectCreationFunc renderObjectCreationFunc =
            [customArgs..., storage, elementsDensity = source.elementsDensity]() mutable
        {
            return std::make_unique<Desc::RenderObject>(
                std::forward<Args>(customArgs)...,
                storage, elementsDensity);
        };

        return std::make_tuple(animatorCreationFunc, renderObjectCreationFunc);
    }
};

} // namespace monsterCatVisualizerOpenGL::utils
