// SPDX-License-Identifier: MIT

#include "utils/vertexBuffer.h"

#include <stdexcept>
#include <utility>

namespace monsterCatVisualizerOpenGL::utils
{

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_id);
    if (m_id == 0)
        throw std::runtime_error("VertexBuffer: failed to create buffer");
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_id(std::exchange(other.m_id, 0)),
      m_sizeBytes(std::exchange(other.m_sizeBytes, 0))
{
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this == &other)
        return *this;

    glDeleteBuffers(1, &m_id);
    m_id = std::exchange(other.m_id, 0);
    m_sizeBytes = std::exchange(other.m_sizeBytes, 0);

    return *this;
}

GLuint VertexBuffer::getId() const noexcept
{
    return m_id;
}

GLsizeiptr VertexBuffer::getSizeBytes() const noexcept
{
    return m_sizeBytes;
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::allocate(GLsizeiptr sizeBytes, GLenum usage, const void* data)
{
    if (sizeBytes < 0)
        throw std::invalid_argument("VertexBuffer: negative allocation size");

    bind();
    glBufferData(GL_ARRAY_BUFFER, sizeBytes, data, usage);
    m_sizeBytes = sizeBytes;
}

void VertexBuffer::clear()
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
}

void VertexBuffer::update(GLintptr offsetBytes, GLsizeiptr sizeBytes, const void* data)
{
    if (offsetBytes < 0 || sizeBytes < 0 ||
        offsetBytes > m_sizeBytes || sizeBytes > m_sizeBytes - offsetBytes)
        throw std::out_of_range("VertexBuffer: update exceeds buffer size");

    if (sizeBytes == 0)
        return;

    if (data == nullptr)
        throw std::invalid_argument("VertexBuffer: null update data");

    bind();
    glBufferSubData(GL_ARRAY_BUFFER, offsetBytes, sizeBytes, data);
}

} // namespace monsterCatVisualizerOpenGL::utils
