#pragma once

#include "pkepch.h"

namespace PurrKatEngine
{
    class Shader
    {
    public:
        static std::string s_WorldVertexSrc;
        static std::string s_WorldCustomColorVertexSrc;
        static std::string s_WorldPositionColorVertexSrc;
        static std::string s_WorldPositionScreenColorVertexSrc;
        static std::string s_IdentityFragmentSrc;
        static std::string s_FlatColorFragmentSrc;
        
    public:
        virtual ~Shader() = default;

        virtual std::string GetName() const = 0;

        static Ref<Shader> Create(const std::string& filePath);
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSrc);
        // static Shader* Create(const std::string& vertexSource, const std::string& fragmentSrc);
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        // Test/Classic shaders
        static Ref<Shader> MakeTextureShader();
        // static Shader* MakePositionColorShader();
        // static Shader* MakeScreenPositionColorShader();
        // static Shader* MakeFlatColorShader();
        // static Shader* MakeCustomColorShader();
        
        // This won't be exposed in the future, because this is too specific.
        virtual void UploadUniformInt(const std::string& name, int value) const = 0;
        virtual void UploadUniformFloat(const std::string& name, float value) const = 0;
        virtual void UploadUniformFloat2(const std::string& str, glm::vec2 color) const = 0;
        virtual void UploadUniformFloat3(const std::string& str, glm::vec3 color) const = 0;
        virtual void UploadUniformFloat4(const std::string& str, glm::vec4 color) const = 0;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const = 0;
    };
    
    class ShaderLibrary
    {
    public:
        void Add(const Ref<Shader>& shader);
        void Add(const Ref<Shader>& shader, const std::string& name);
        Ref<Shader> Load(const std::string& filePath);
        Ref<Shader> Load(const std::string& filePath, const std::string& name);
        
        Ref<Shader> Get(const std::string& name);
        
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
}
