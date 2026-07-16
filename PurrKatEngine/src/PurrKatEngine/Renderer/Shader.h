#pragma once

#include "pkepch.h"

namespace PurrKatEngine
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual std::string GetName() const = 0;

        static Shader* Create(const std::string& filePath);
        static Shader* Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSrc);
        // static Shader* Create(const std::string& vertexSource, const std::string& fragmentSrc);
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        // Test/Classic shaders
        static Shader* MakeTextureShader();
        // static Shader* MakePositionColorShader();
        // static Shader* MakeScreenPositionColorShader();
        // static Shader* MakeFlatColorShader();
        // static Shader* MakeCustomColorShader();
        
        virtual void SetUniformInt(const std::string& name, int value) const = 0;
        virtual void SetUniformFloat(const std::string& name, float value) const = 0;
        virtual void SetUniformFloat2(const std::string& name, glm::vec2 color) const = 0;
        virtual void SetUniformFloat3(const std::string& name, glm::vec3 color) const = 0;
        virtual void SetUniformFloat4(const std::string& name, glm::vec4 color) const = 0;
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const = 0;
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
