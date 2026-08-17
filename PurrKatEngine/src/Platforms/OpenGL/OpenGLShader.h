#pragma once

#include "pkepch.h"

#include "glad/glad.h"
#include "PurrKatEngine/Renderer/Shader.h"

namespace PurrKatEngine
{
    class OpenGLShader : public Shader
    {
    public:
        explicit OpenGLShader(const std::string& filePath);
        explicit OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader() override;
        
        static GLenum ShaderTypeFromString(const std::string& type);

        std::string GetName() const override { return m_ShaderName; }
        
        void Bind() const override;
        void Unbind() const override;
        
        void SetUniformInt(const std::string& name, int value) const override;
        void SetUniformIntArray(const std::string& name, int* values, uint32_t count) const override;
        void SetUniformFloat(const std::string& name, float value) const override;
        void SetUniformFloat2(const std::string& name, glm::vec2 vec) const override;
        void SetUniformFloat3(const std::string& name, glm::vec3 vec) const override;
        void SetUniformFloat4(const std::string& name, glm::vec4 vec) const override;
        void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const override;
        
    private:
        static std::string ReadFile(const std::string& filePath);
        static std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    
    private:
        std::string m_ShaderName;
        uint32_t m_RendererID = 0;
    };
}
