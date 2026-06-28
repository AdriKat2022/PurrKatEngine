#pragma once

#include "pkepch.h"

namespace PurrKatEngine
{
    class Shader
    {
    public:
        static std::string s_TextureShaderVertexSrc;
        static std::string s_TextureShaderFramgmentSrc;
        static std::string s_WorldVertexSrc;
        static std::string s_WorldCustomColorVertexSrc;
        static std::string s_WorldPositionColorVertexSrc;
        static std::string s_WorldPositionScreenColorVertexSrc;
        static std::string s_IdentityFragmentSrc;
        static std::string s_FlatColorFragmentSrc;
        
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        // This won't be exposed in the future, because this is too specific.
        virtual void UploadUniformInt(const std::string& name, int value) const = 0;
        virtual void UploadUniformFloat(const std::string& name, float value) const = 0;
        virtual void UploadUniformFloat2(const std::string& str, glm::vec2 color) const = 0;
        virtual void UploadUniformFloat3(const std::string& str, glm::vec3 color) const = 0;
        virtual void UploadUniformFloat4(const std::string& str, glm::vec4 color) const = 0;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const = 0;

        static Shader* Create(const std::string& vertexSource, const std::string& fragmentSrc);
        
        // Test/Classic shaders
        static Shader* MakeTextureShader();
        static Shader* MakePositionColorShader();
        static Shader* MakeScreenPositionColorShader();
        static Shader* MakeFlatColorShader();
        static Shader* MakeCustomColorShader();

    protected:
        uint32_t m_RendererID = 0;
    };
}
