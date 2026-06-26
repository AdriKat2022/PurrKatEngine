#pragma once

#include "pkepch.h"

#include "PurrKatEngine/Renderer/Shader.h"

namespace PurrKatEngine
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;
        
        void UploadUniformInt(const std::string& name, int value) const override;
        void UploadUniformFloat(const std::string& name, float value) const override;
        void UploadUniformFloat2(const std::string& name, glm::vec2 vec) const override;
        void UploadUniformFloat3(const std::string& name, glm::vec3 vec) const override;
        void UploadUniformFloat4(const std::string& name, glm::vec4 vec) const override;
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const override;
    };
}
