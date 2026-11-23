// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "models/container.h"

namespace monsterCatVisualizerOpenGL::rendering
{

template <typename T, typename U>
class RenderContainerHandler
{
public:
    using DataModel = T;
    using DataModelGpu = U;

    using Storage = monsterCatVisualizer::models::Container<DataModel>;
    using PatchStorage = typename Storage::PatchStorage;
    using StoragePtr = std::shared_ptr<Storage>;
    using StorageGpu = std::vector<DataModelGpu>;

    RenderContainerHandler(StoragePtr storagePtr)
        : m_storagePtr(std::move(storagePtr))
    {
        if (!m_storagePtr)
            throw std::runtime_error("RenderContainerHandler: empty storage");
    }

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
    bool needsFullUpload() const noexcept
    {
        return m_needsFullUpload;
    }

    void initialize(std::size_t desiredCount)
    {
        m_storageGpu.reserve(desiredCount);
        Storage::reservePatch(desiredCount, m_lastPatch);
        setInitialData();
    }

    /**
     * Synchronize rendering state with animation state.
     * @return true if there are some changes.
     */
    bool synchronize()
    {
        if (!m_storagePtr->acquirePatch(m_lastPatch))
            return false;

        if (m_requiresResync || !applyPatch(m_lastPatch))
            setInitialData();
        else
            m_needsFullUpload = m_lastPatch.clearFlag;

        m_requiresResync = false;
        return true;
    }

    void clear()
    {
        m_storageGpu.clear();
        m_lastPatch.reset();
        m_requiresResync = true;
        m_needsFullUpload = true;
    }

private:
    void setInitialData()
    {
        std::vector<DataModel> initialData;
        m_storagePtr->getInitialData(initialData);
        m_lastPatch.reset();

        m_storageGpu.clear();
        appendElements(initialData);
        m_needsFullUpload = true;
        m_requiresResync = false;
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

    void appendElements(const std::vector<DataModel>& appended)
    {
        const auto oldSize = m_storageGpu.size();
        m_storageGpu.resize(oldSize + appended.size());

        std::transform(
            appended.begin(),
            appended.end(),
            m_storageGpu.begin() + oldSize,
            DataModelGpu::convert);
    }

    StoragePtr m_storagePtr;
    StorageGpu m_storageGpu;
    PatchStorage m_lastPatch{};
    bool m_needsFullUpload{};
    bool m_requiresResync{};
};

} // namespace monsterCatVisualizerOpenGL::rendering
