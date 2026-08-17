#include "pkepch.h"
#include "OpenGLShader.h"

#include "PurrKatEngine/Logs/InternalLog.h"
#include "PurrKatEngine/Profiling/Profiler.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace PurrKatEngine
{
	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string shaderFileContents = ReadFile(filePath);
		auto shaderSources = Preprocess(shaderFileContents);
		Compile(shaderSources);
		
		// Extract the shader name from the filepath.
		auto lastSlashPos = filePath.find_last_of("/\\");
		lastSlashPos = lastSlashPos == std::string::npos ? 0 : lastSlashPos + 1;
		auto lastDotPos = filePath.rfind('.');
		auto count = lastDotPos == std::string::npos ? filePath.size() - lastSlashPos : lastDotPos - lastSlashPos;
		
		m_ShaderName = filePath.substr(lastSlashPos, count);
	}

    OpenGLShader::OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_ShaderName(std::move(name))
    {
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
    }

    OpenGLShader::~OpenGLShader()
    {
    }

    GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")	return GL_VERTEX_SHADER;
		if (type == "fragment")	return GL_FRAGMENT_SHADER;
		if (type == "pixel")	return GL_FRAGMENT_SHADER;
		
		PKE_CORE_ERROR("Unknown shader type: {}", type);
		return 0;
	}

    void OpenGLShader::Bind() const
    {
		PROFILE_FUNCTION();
    	glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
    	glUseProgram(0);
    }

    void OpenGLShader::SetUniformInt(const std::string& name, int value) const
    {
    	PROFILE_FUNCTION();
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1i(location, value);
    }

    void OpenGLShader::SetUniformIntArray(const std::string& name, int* values, uint32_t count) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, (GLint)count, values);
	}

    void OpenGLShader::SetUniformFloat(const std::string& name, float value) const
    {
    	PROFILE_FUNCTION();
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1f(location, value);
    }

    void OpenGLShader::SetUniformFloat2(const std::string& name, glm::vec2 vec) const
    {
    	PROFILE_FUNCTION();
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform2f(location, vec.r, vec.g);
    }
	
    void OpenGLShader::SetUniformFloat3(const std::string& name, glm::vec3 vec) const
    {
    	PROFILE_FUNCTION();
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform3f(location, vec.r, vec.g, vec.b);
    }

    void OpenGLShader::SetUniformFloat4(const std::string& name, glm::vec4 vec) const
    {
    	PROFILE_FUNCTION();
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform4f(location, vec.r, vec.g, vec.b, vec.a);
    }

    void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) const
    {
    	PROFILE_FUNCTION();
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream stream(filePath, std::ios::in, std::ios::binary);
		if (stream)
		{
			stream.seekg(0, std::ios::end);
			result.resize(stream.tellg());
			stream.seekg(0, std::ios::beg);
			stream.read(&result[0], result.size());
			stream.close();
		}
		else
		{
			PKE_CORE_ERROR("Could not open file: '{}'", filePath);
		}
		
		return result;
	}

    std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			
			PKE_CORE_ASSERT(eol != std::string::npos, "Syntax error while preprocessing shader file.");
			
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			
			GLenum shaderTypeEnum = ShaderTypeFromString(type);
			PKE_CORE_ASSERT(shaderTypeEnum, "'{}' is an invalid shader type.", type);
			
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[shaderTypeEnum] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		
		return shaderSources;
	}
	
    void OpenGLShader::Compile(const ::std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Get a program object.
		GLuint program = glCreateProgram();
		
		std::array<GLenum, 2> glShaderIds;
		PKE_CORE_ASSERT(shaderSources.size() <= 2, "Too many shaders to compile ({}).", shaderSources.size());
		
		int shaderCount = 0;
		
		for (auto& [type, src] : shaderSources)
		{
			GLuint shader = glCreateShader(type);
			const GLchar* source = src.c_str();
			
			glShaderSource(shader, 1, &source, nullptr);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			
			if(isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());
			
				// We don't need the shader anymore, since it failed to initialize.
				glDeleteShader(shader);

				PKE_CORE_ERROR("{} shader compilation failure: {}", type, infoLog.data());
				PKE_CORE_ASSERT(false, "{} shader compilation failure.", type)
				return;
			}
			
			// Attach shader to our program
			glAttachShader(program, shader);
			glShaderIds[shaderCount++] = shader;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		m_RendererID = program;

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
			
			for (auto shaderId : glShaderIds)
			{
				if (shaderId != 0) glDeleteShader(shaderId);
			}

			PKE_CORE_ERROR("Program Shader linking failure: {}", infoLog.data());
			PKE_CORE_ASSERT(false, "Program Shader linking failure.")
			return;
		}

		// Always detach shaders after a successful link.
		for (auto shaderId : glShaderIds)
		{
			glDetachShader(m_RendererID, shaderId);
		}
	}
}
