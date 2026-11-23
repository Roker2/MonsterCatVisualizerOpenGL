// SPDX-License-Identifier: MIT

#pragma once

#include <glad/gl.h>

namespace monsterCatVisualizerOpenGL::utils
{

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    GLuint getId() const noexcept;
    GLsizeiptr getSizeBytes() const noexcept;

    void bind() const;
    static void unbind();

    /** Allocate/replace the entire GPU data store. Data may be null. */
    void allocate(GLsizeiptr sizeBytes, GLenum usage = GL_DYNAMIC_DRAW,
        const void* data = nullptr);
    
    /** Clear the entire GPU data store. Doesn't delete buffer itself. */
    void clear();

    /** Update a byte range without reallocating the data store. */
    void update(GLintptr offsetBytes, GLsizeiptr sizeBytes, const void* data);

private:
    GLuint m_id{};
    GLsizeiptr m_sizeBytes{};
};

} // namespace monsterCatVisualizerOpenGL::utils
