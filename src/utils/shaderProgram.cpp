// SPDX-License-Identifier: MIT

#include "utils/shaderProgram.h"

#include <stdexcept>
#include <utility>

#include "utils/shader.h"

namespace monsterCatVisualizerOpenGL::utils
{

ShaderProgram::ShaderProgram(
    std::string_view vertexSource,
    std::string_view fragmentSource)
{
    std::string error;
    m_programId = createShaderProgram(vertexSource, fragmentSource, error);

    if (m_programId == 0)
        throw std::runtime_error(
            "ShaderProgram: failed to create shader program. error: " + error);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : m_programId(std::exchange(other.m_programId, 0)),
    m_uniforms(std::move(other.m_uniforms))
{
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this == &other)
        return *this;

    glDeleteProgram(m_programId);
    m_programId = std::exchange(other.m_programId, 0);
    m_uniforms = std::move(other.m_uniforms);

    return *this;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_programId);
}

void ShaderProgram::bind() const
{
    glUseProgram(m_programId);
}

void ShaderProgram::unbind()
{
    glUseProgram(0);
}

void ShaderProgram::validate() const
{
    glValidateProgram(m_programId);

    GLint params;
    glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &params);

    if (params == GL_FALSE)
    {
        GLint logSize = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &logSize);

        std::string infoLog(logSize, '\0');
        GLsizei written = 0;

        glGetProgramInfoLog(m_programId, logSize, &written, infoLog.data());
        infoLog.resize(written);

        throw std::runtime_error("ShaderProgram: failed to validate: " + infoLog);
    }
}

void ShaderProgram::setUniform(std::string_view uniformName, GLint value)
{
    glUniform1i(getUniformLocation(uniformName), value);
}

void ShaderProgram::setUniform(std::string_view uniformName, GLfloat value)
{
    glUniform1f(getUniformLocation(uniformName), value);
}

void ShaderProgram::setUniform(std::string_view uniformName,
    const monsterCatVisualizer::primitives::Vector2<>& value)
{
    glUniform2f(getUniformLocation(uniformName), value.x, value.y);
}

void ShaderProgram::setUniform(std::string_view uniformName,
    const monsterCatVisualizer::primitives::ColorRGB<>& value)
{
    glUniform3f(getUniformLocation(uniformName),
        value.r / 255.0f,
        value.g / 255.0f,
        value.b / 255.0f);
}

void ShaderProgram::setUniform(std::string_view uniformName,
    const monsterCatVisualizer::primitives::ColorRGBA<>& value)
{
    glUniform4f(getUniformLocation(uniformName),
        value.r / 255.0f,
        value.g / 255.0f,
        value.b / 255.0f,
        value.a);
}

void ShaderProgram::createUniforms(const std::vector<std::string>& uniformNames)
{
    for (const auto& uniformName : uniformNames)
		createUniform(uniformName);
}

void ShaderProgram::createUniform(std::string_view uniformName)
{
    if (uniformName.empty())
        throw std::invalid_argument("ShaderProgram: createUniform with empty uniformName");
    if (m_uniforms.count(uniformName) > 0)
        throw std::invalid_argument("ShaderProgram: creating the same uniform twice is not allowed");

    std::string name(uniformName);
	GLint uniformLocation = glGetUniformLocation(m_programId, name.c_str());
	if (uniformLocation < 0)
	{
        throw std::runtime_error("ShaderProgram: Could not find uniform: name=" +
            name + ", id=" + std::to_string(uniformLocation));
	}

    m_uniforms.emplace(std::move(name), uniformLocation);
}

GLint ShaderProgram::getUniformLocation(std::string_view uniformName) const
{
    auto it = m_uniforms.find(uniformName);
    if (it == m_uniforms.end())
        throw std::invalid_argument("ShaderProgram: getUniformLocation with invalid uniformName=" +
            std::string(uniformName));
    return it->second;
}

GLuint ShaderProgram::createShaderProgram(std::string_view vertexSource,
    std::string_view fragmentSource,
    std::string& error)
{
    const auto vertexShader = Shader(GL_VERTEX_SHADER, vertexSource);
	const auto fragmentShader = Shader(GL_FRAGMENT_SHADER, fragmentSource);

    const GLuint programId = glCreateProgram();
    if (programId == 0)
    {
        error = "failed to create shader program";
		return 0;
    }

    vertexShader.attachShader(programId);
	fragmentShader.attachShader(programId);

    glLinkProgram(programId);

    GLint params;
    glGetProgramiv(programId, GL_LINK_STATUS, &params);

    if (params == GL_FALSE)
    {
        GLint logSize = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

        std::string infoLog(logSize, '\0');
        GLsizei written = 0;

        glGetProgramInfoLog(programId, logSize, &written, infoLog.data());
        infoLog.resize(written);

        glDeleteProgram(programId);
        error = "failed to link: " + infoLog;
		return 0;
    }

    vertexShader.detachShader(programId);
	fragmentShader.detachShader(programId);

    return programId;
}

} // namespace monsterCatVisualizerOpenGL::utils
