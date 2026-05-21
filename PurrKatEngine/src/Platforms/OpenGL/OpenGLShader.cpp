#include "pkepch.h"
#include "OpenGLShader.h"

#include "PurrKatEngine/Logs/InternalLog.h"
#include "glad/glad.h"
#include <backends/imgui_impl_opengl3_loader.h>
#include <glm/gtc/type_ptr.hpp>

namespace PurrKatEngine
{
    OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());
			
			// We don't need the shader anymore, since it failed to initialize.
			glDeleteShader(vertexShader);

			PKE_CORE_ERROR("Vertex shader compilation failure: {}", infoLog.data());
			PKE_CORE_ASSERT(false, "Vertex shader compilation failure.")
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());
			
			// We don't need the shader anymore. Either of them. Don't leak shaders.
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			PKE_CORE_ERROR("Fragment shader compilation failure: {}", infoLog.data());
			PKE_CORE_ASSERT(false, "Fragment shader compilation failure.")
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, infoLog.data());
			
			// We don't need the program anymore, nor the shaders.
			glDeleteProgram(m_RendererID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			PKE_CORE_ERROR("Program Shader linking failure: {}", infoLog.data());
			PKE_CORE_ASSERT(false, "Program Shader linking failure.")
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
    }

    OpenGLShader::~OpenGLShader()
    {
    }

    void OpenGLShader::Bind() const
    {
    	glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
    	glUseProgram(0);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value) const
    {
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value) const
    {
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, glm::vec4 color) const
    {
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform4f(location, color.r, color.g, color.b, color.a);
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
    {
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}
