// SPDX-License-Identifier: MIT

#include "utils/shader.h"

#include <stdexcept>

namespace monsterCatVisualizerOpenGL::utils
{

Shader::Shader(GLenum type, std::string_view source)
{
    std::string error;
    m_id = createShader(type, source, error);

    if (m_id == 0)
        throw std::runtime_error(
            "Shader: failed to create shader with type=" + std::to_string(type) +
            ". error: " + error);
}

Shader::~Shader()
{
    glDeleteShader(m_id);
}

GLuint Shader::getId() const noexcept
{
	return m_id;
}

void Shader::attachShader(GLuint shaderProgram) const
{
	if (shaderProgram == 0)
		throw std::invalid_argument("Shader: attachShader with invalid shaderProgram");
	glAttachShader(shaderProgram, m_id);
}

void Shader::detachShader(GLuint shaderProgram) const
{
	if (shaderProgram == 0)
		throw std::invalid_argument("Shader: detachShader with invalid shaderProgram");
	glDetachShader(shaderProgram, m_id);
}

GLuint Shader::createShader(GLenum type, std::string_view source, std::string& error)
{
    if (source.empty())
    {
        error = "empty input source";
        return 0;
    }

	const GLuint id = glCreateShader(type);
	if (id == 0)
    {
        error = "failed to create shader";
		return 0;
    }

	const GLchar* stringNative = source.data();
	const GLint length = static_cast<GLint>(source.size());
	glShaderSource(id, 1, &stringNative, &length);
	glCompileShader(id);

	GLint params;
	glGetShaderiv(id, GL_COMPILE_STATUS, &params);

	if (params == GL_FALSE)
	{
        GLint logSize = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);

        std::string infoLog(logSize, '\0');
        GLsizei written = 0;

        glGetShaderInfoLog(id, logSize, &written, infoLog.data());
        infoLog.resize(written);

        glDeleteShader(id);
        error = "failed to compile: " + infoLog;
		return 0;
	}

    return id;
}

} // namespace monsterCatVisualizerOpenGL::utils
