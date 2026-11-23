// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <glad/gl.h>

#include "utils/shaderProgram.h"
#include "utils/vertexArray.h"
#include "utils/vertexBuffer.h"

namespace monsterCatVisualizerOpenGL::rendering
{

template <typename T>
class RenderVisualHandler
{
public:
    using DataModelGpu = T;

    using StorageGpu = std::vector<DataModelGpu>;
    using UniformNames = std::vector<std::string>;
    using VertexAttributes = std::vector<utils::VertexAttribute>;

    template <typename Value>
    using UniformPair = std::pair<std::string_view, Value>;

    static constexpr std::size_t maxElements = std::min(
        static_cast<std::size_t>(std::numeric_limits<GLsizei>::max()),
        static_cast<std::size_t>(std::numeric_limits<GLsizeiptr>::max()) /
            sizeof(DataModelGpu));

    RenderVisualHandler(std::string_view vertexSource,
        std::string_view fragmentSource)
        : m_shaderProgram(vertexSource, fragmentSource)
    {
    }

    GLsizei getRenderedCount() const noexcept
    {
        return m_renderedCount;
    }

    std::size_t getCapacity() const noexcept
    {
        return static_cast<std::size_t>(getSizeBytes() / sizeof(DataModelGpu));
    }

    GLsizeiptr getSizeBytes() const noexcept
    {
        return m_vertexBuffer.getSizeBytes();
    }

    void initialize(std::size_t desiredCount,
        const UniformNames& uniformNames,
        const VertexAttributes& attributes)
    {
        if (desiredCount > maxElements)
            throw std::length_error("RenderVisualHandler: too many elements");

        m_shaderProgram.createUniforms(uniformNames);

        for (const auto& attribute : attributes)
        {
            m_vertexArray.addAttribute(m_vertexBuffer, attribute);
        }

#ifndef NDEBUG
        m_shaderProgram.bind();
        m_vertexArray.bind();
        m_shaderProgram.validate();
        utils::ShaderProgram::unbind();
#endif

        utils::VertexArray::unbind();
        utils::VertexBuffer::unbind();

        if (desiredCount > 0)
        {
            m_vertexBuffer.allocate(static_cast<GLsizeiptr>(desiredCount * sizeof(DataModelGpu)),
                GL_DYNAMIC_DRAW);
        }
    }

    /** Upload an entire snapshot (initialization or patch fallback). */
    void synchronize(const StorageGpu& storageGpu)
    {
        ensureCapacity(storageGpu.size());
        uploadAll(storageGpu);
        m_renderedCount = static_cast<GLsizei>(storageGpu.size());
    }

    /** Upload only the modified ranges, unless the buffer must be replaced. */
    template <typename ModifiedRanges>
    void synchronize(const StorageGpu& storageGpu,
        const ModifiedRanges& modifiedRanges,
        bool forceFullUpload)
    {
        const bool reallocated = ensureCapacity(storageGpu.size());

        if (forceFullUpload || reallocated)
        {
            uploadAll(storageGpu);
        }
        else
        {
            for (const auto& range : modifiedRanges)
            {
                // Earlier removals can leave ranges past the final vector size.
                if (range.index >= storageGpu.size())
                    continue;

                const std::size_t count = std::min(range.count, storageGpu.size() - range.index);
                if (count == 0)
                    continue;

                m_vertexBuffer.update(
                    static_cast<GLintptr>(range.index * sizeof(DataModelGpu)),
                    static_cast<GLsizeiptr>(count * sizeof(DataModelGpu)),
                    storageGpu.data() + range.index);
            }
        }

        m_renderedCount = static_cast<GLsizei>(storageGpu.size());
    }

    void clear()
    {
        m_renderedCount = 0;
    }

    template <typename... Args>
    void prepareRendering(Args&&... uniformPairs)
    {
        if (m_renderedCount == 0)
            return;

        m_shaderProgram.bind();
        (setUniform(uniformPairs), ...);
        m_vertexArray.bind();
    }

    template <typename U>
    void setUniform(const UniformPair<U>& pair)
    {
        m_shaderProgram.setUniform(pair.first, pair.second);
    }

private:
    /** @return true if a larger GPU data store was allocated. */
    bool ensureCapacity(std::size_t requiredCount)
    {
        if (requiredCount > maxElements)
            throw std::length_error("RenderVisualHandler: too many elements");

        if (requiredCount <= getCapacity())
            return false;

        std::size_t newCapacity = std::max(getCapacity(), std::size_t{1});
        while (newCapacity < requiredCount)
        {
            if (newCapacity > maxElements / 2)
            {
                newCapacity = maxElements;
                break;
            }
            newCapacity *= 2;
        }

        m_vertexBuffer.allocate(
            static_cast<GLsizeiptr>(newCapacity * sizeof(DataModelGpu)), GL_DYNAMIC_DRAW);
        return true;
    }

    void uploadAll(const StorageGpu& storageGpu)
    {
        if (storageGpu.empty())
            return;

        m_vertexBuffer.update(0,
            static_cast<GLsizeiptr>(storageGpu.size() * sizeof(DataModelGpu)),
            storageGpu.data());
    }

    utils::ShaderProgram m_shaderProgram;
    utils::VertexBuffer m_vertexBuffer;
    utils::VertexArray m_vertexArray;
    GLsizei m_renderedCount{};
};

} // namespace monsterCatVisualizerOpenGL::rendering
