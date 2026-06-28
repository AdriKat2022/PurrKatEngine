#include "pkepch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace PurrKatEngine
{
    Ref<Shader> Shader::Create(const std::string& filePath)
    {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:
                PKE_CORE_ASSERT(false, "RendererAPI::None is currently not supported.")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return MakeRef<OpenGLShader>(filePath);
        }
        
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSrc)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:
                PKE_CORE_ASSERT(false, "RendererAPI::None is currently not supported.")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return MakeRef<OpenGLShader>(name, vertexSource, fragmentSrc);
        }
        
        return nullptr;
    }
    
    Ref<Shader> Shader::MakeTextureShader()
    {
        return Create("assets/shaders/Texture.glsl");
    }

    // -------------- SHADER LIBRARY ------------------
    
    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        const auto& name = shader->GetName();
        PKE_CORE_ASSERT(!m_Shaders.contains(name), "Shader with name '{}' already exists in the library.", name);
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader, const std::string& name)
    {
        PKE_CORE_ASSERT(!m_Shaders.contains(name), "Shader with name '{}' already exists in the library.", name);
        m_Shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
    {
        auto shader = Shader::Create(filePath);
        Add(shader);
        return shader;
    }
    
    Ref<Shader> ShaderLibrary::Load(const std::string& filePath, const std::string& name)
    {
        auto shader = Shader::Create(filePath);
        Add(shader, name);
        return shader;
    }
    
    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        PKE_CORE_ASSERT(m_Shaders.contains(name), "Shader with name '{}' was not found in the library.", name);
        
        return m_Shaders[name];
    }
}
