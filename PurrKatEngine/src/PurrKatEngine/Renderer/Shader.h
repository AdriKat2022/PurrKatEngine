#pragma once

#include "pkepch.h"

namespace PurrKatEngine
{
    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void UploadUniformInt(const std::string& name, int value) const = 0;
        virtual void UploadUniformFloat(const std::string& name, float value) const = 0;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const = 0;
        
    protected:
        uint32_t m_RendererID;
    };
}
