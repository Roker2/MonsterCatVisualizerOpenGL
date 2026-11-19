// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "animation/baseAnimationHandler.h"
#include "utils/mathUtils.h"
#include "models/container.h"

namespace monsterCatVisualizerOpenGL::animation
{

template <typename T, typename U>
class ContainerAnimationHandler : public BaseAnimationHandler
{
public:
    using Storage = monsterCatVisualizer::models::Container<T>;
    using PatchStorage = typename Storage::PatchStorage;
    using StoragePtr = std::shared_ptr<Storage>;
    using StorageGpu = std::vector<U>;

    StoragePtr getStorage() const
    {
        return m_storagePtr;
    }
    const StorageGpu& getStorageGpu() const
    {
        return m_storageGpu;
    }
    const PatchStorage& getLastPatch() const
    {
        return m_lastPatch;
    }

    using BaseAnimationHandler::initialize;

    void initialize(std::size_t desiredCount) override
    {
        m_storageGpu.reserve(desiredCount);
        Storage::reservePatch(desiredCount, m_lastPatch);

        setInitialData();
    }

    void clear() override
    {
        m_storageGpu.clear();
    }

    /**
     * An update is not atomic across the patch check, removal, and append.
     * Intended for one producer and one consumer.
     * No other thread may mutate this storage while the producer is updating it.
     */
    void handleUpdate() override
    {
        if (!m_storagePtr->hasPatch())
            return;

        m_storagePtr->acquirePatch(m_lastPatch);

        if (!applyPatch(m_lastPatch))
            setInitialData();
    }

    std::size_t calculateDesiredCount(std::int32_t viewportWidth,
        std::int32_t viewportHeight) const override
    {
        return monsterCatVisualizer::utils::SpatialMathUtils::calculateDesiredCount(
            m_elementsDensity, viewportWidth, viewportHeight);
    }

protected:
    ContainerAnimationHandler(StoragePtr storagePtr, float elementsDensity)
        : m_storagePtr(std::move(storagePtr)),
          m_elementsDensity(elementsDensity)
    {
        if (!m_storagePtr)
            throw std::runtime_error("ContainerAnimationHandler: empty storage");

        const bool elementsDensityValid = std::isfinite(elementsDensity) &&
            elementsDensity > T::elementsDensityMin;
        if (!elementsDensityValid)
            throw std::runtime_error("ContainerAnimationHandler: invalid elementsDensity");
    }

    void setInitialData()
    {
        std::vector<T> initialData;
        m_storagePtr->getInitialData(initialData);
        m_lastPatch.reset();

        m_storageGpu.clear();

        appendElements(initialData);
    }

    bool applyPatch(const PatchStorage& patch)
    {
        if (patch.clearFlag)
        {
            m_storageGpu.clear();
        }
        else
        {
            for (const auto& operation : patch.removeOperations)
            {
                if (!monsterCatVisualizer::models::applyRemoveOperation(m_storageGpu, operation))
                    return false;
            }
        }

        appendElements(patch.appended);

        return true;
    }

    void appendElements(const std::vector<T>& appended)
    {
        const auto oldSize = m_storageGpu.size();
        m_storageGpu.resize(oldSize + appended.size());

        std::transform(
            appended.begin(),
            appended.end(),
            m_storageGpu.begin() + oldSize,
            U::convert);
    }

    StoragePtr m_storagePtr;
    StorageGpu m_storageGpu;
    PatchStorage m_lastPatch{};
    float m_elementsDensity{};
};

} // namespace monsterCatVisualizerOpenGL::animation
