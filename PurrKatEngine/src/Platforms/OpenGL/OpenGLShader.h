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

    private:
        uint32_t m_RendererID; // Allows referencing this object in OpenGL
    };
}
