// SPDX-License-Identifier: MIT

#include "utils/vertexArray.h"

#include <stdexcept>
#include <utility>

#include "utils/vertexBuffer.h"

namespace monsterCatVisualizerOpenGL::utils
{

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
    if (m_id == 0)
        throw std::runtime_error("VertexArray: failed to create vertex array");
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_id(std::exchange(other.m_id, 0))
{
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this == &other)
        return *this;

    glDeleteVertexArrays(1, &m_id);
    m_id = std::exchange(other.m_id, 0);

    return *this;
}

GLuint VertexArray::getId() const noexcept
{
    return m_id;
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::unbind()
{
    glBindVertexArray(0);
}

void VertexArray::addAttribute(const VertexBuffer& buffer, const VertexAttribute& attribute)
{
    if (attribute.components < 1 || attribute.components > 4 || attribute.stride < 0)
        throw std::invalid_argument("VertexArray: invalid vertex attribute layout");

    bind();
    buffer.bind();

    const auto* offset = reinterpret_cast<const void*>(attribute.offset);

    if (attribute.kind == VertexAttributeKind::Integer)
    {
        glVertexAttribIPointer(attribute.location, attribute.components,
                               attribute.type, attribute.stride, offset);
    }
    else
    {
        glVertexAttribPointer(attribute.location, attribute.components,
                              attribute.type, attribute.normalized,
                              attribute.stride, offset);
    }

    glEnableVertexAttribArray(attribute.location);
    glVertexAttribDivisor(attribute.location, attribute.divisor);
}

} // namespace monsterCatVisualizerOpenGL::utils
