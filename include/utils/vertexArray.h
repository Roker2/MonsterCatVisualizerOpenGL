// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <glad/gl.h>

namespace monsterCatVisualizerOpenGL::utils
{

class VertexBuffer;

enum class VertexAttributeKind
{
    Floating, // Float or normalized integer data, exposed to GLSL as float/vecN.
    Integer   // Integer data, exposed to GLSL as int/ivecN or uint/uvecN.
};

struct VertexAttribute
{
    GLuint location{};              // Index of attribute in vertex shader
    GLint components{};             // Count of components (for vecN)
    GLenum type{GL_FLOAT};          // Data type
    GLboolean normalized{GL_FALSE}; // Is integer data normalized when transposed as float
    GLsizei stride{};               // Bytes between successive vertices/instances; 0 = tightly packed.
    std::size_t offset{};           // Byte offset of the field within the buffer.
    GLuint divisor{};               // 0 = per vertex; 1 = per instance.
    VertexAttributeKind kind{VertexAttributeKind::Floating};
};

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    GLuint getId() const noexcept;

    void bind() const;
    static void unbind();

    /**
     * Captures the specified buffer binding and attribute layout into this VAO.
     * The buffer must remain alive while this layout is in use.
     */
    void addAttribute(const VertexBuffer& buffer, const VertexAttribute& attribute);

private:
    GLuint m_id{};
};

} // namespace monsterCatVisualizerOpenGL::utils
