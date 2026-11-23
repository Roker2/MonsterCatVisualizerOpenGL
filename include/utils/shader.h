// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>
#include <string>
#include <glad/gl.h>

namespace monsterCatVisualizerOpenGL::utils
{

class Shader
{
public:
	Shader(GLenum type, std::string_view source);

    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;

	~Shader();

	GLuint getId() const noexcept;

	void attachShader(GLuint shaderProgram) const;
	void detachShader(GLuint shaderProgram) const;

private:
    static GLuint createShader(GLenum type, std::string_view source,
        std::string& error);

	GLuint m_id{};
};

} // namespace monsterCatVisualizerOpenGL::utils
