// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>
#include <utility>

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

        // TODO: make incremental transferring data to VBO
    }

    void synchronize(const StorageGpu& storageGpu)
    {
        if (storageGpu.size() > maxElements)
            throw std::length_error("RenderVisualHandler: too many elements");

        const auto bytes = static_cast<GLsizeiptr>(storageGpu.size() * sizeof(DataModelGpu));
        m_vertexBuffer.allocate(bytes, GL_DYNAMIC_DRAW, storageGpu.empty() ? nullptr : storageGpu.data());
        m_renderedCount = static_cast<GLsizei>(storageGpu.size());
    }

    void clear()
    {
        m_vertexBuffer.clear();
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
    utils::ShaderProgram m_shaderProgram;
    utils::VertexBuffer m_vertexBuffer;
    utils::VertexArray m_vertexArray;
    GLsizei m_renderedCount{};
};

} // namespace monsterCatVisualizerOpenGL::rendering
