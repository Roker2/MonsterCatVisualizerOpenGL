// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <map>
#include <glad/gl.h>

#include "primitives/color.h"
#include "primitives/vector2.h"

namespace monsterCatVisualizerOpenGL::utils
{

class ShaderProgram
{
public:
    ShaderProgram(std::string_view vertexSource,
        std::string_view fragmentSource);

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    ~ShaderProgram();

    void bind() const;
    static void unbind();
    void validate() const;

	void setUniform(std::string_view uniformName, GLint value);
	void setUniform(std::string_view uniformName, GLfloat value);
	void setUniform(std::string_view uniformName,
        const monsterCatVisualizer::primitives::Vector2<>& value);
    void setUniform(std::string_view uniformName,
        const monsterCatVisualizer::primitives::ColorRGB<>& value);
    void setUniform(std::string_view uniformName,
        const monsterCatVisualizer::primitives::ColorRGBA<>& value);

    void createUniforms(const std::vector<std::string>& uniformNames);
	void createUniform(std::string_view uniformName);

private:
    GLint getUniformLocation(std::string_view uniformName) const;

    static GLuint createShaderProgram(std::string_view vertexSource,
        std::string_view fragmentSource,
        std::string& error);

    GLuint m_programId{};
    std::map<std::string, GLint, std::less<>> m_uniforms;
};

} // namespace monsterCatVisualizerOpenGL::utils
