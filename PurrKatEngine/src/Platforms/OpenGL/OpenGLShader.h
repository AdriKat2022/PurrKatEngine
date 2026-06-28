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
        
        void UploadUniformInt(const std::string& name, int value) const override;
        void UploadUniformFloat(const std::string& name, float value) const override;
        void UploadUniformFloat2(const std::string& name, glm::vec2 vec) const override;
        void UploadUniformFloat3(const std::string& name, glm::vec3 vec) const override;
        void UploadUniformFloat4(const std::string& name, glm::vec4 vec) const override;
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const override;
        
    private:
        static std::string ReadFile(const std::string& filePath);
        static std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    
    private:
        std::string m_ShaderName;
        uint32_t m_RendererID = 0;
    };
}
