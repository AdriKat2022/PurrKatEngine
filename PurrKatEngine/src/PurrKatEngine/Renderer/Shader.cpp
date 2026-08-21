#include "pkepch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace PurrKatEngine
{
    Shader* Shader::Create(const std::string& filePath)
    {
        SWITCH_ON_RENDERER_API(
            API_CASE_NONE_NOT_SUPPORTED
            API_CASE_OPENGL return new OpenGLShader(filePath);
        )
    }

    Shader* Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSrc)
    {
        SWITCH_ON_RENDERER_API(
            API_CASE_NONE_NOT_SUPPORTED
            API_CASE_OPENGL return new OpenGLShader(name, vertexSource, fragmentSrc);
        )
    }

    Shader* Shader::MakeTextureShader()
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
        auto shader = CreateRef(Shader::Create(filePath));
        Add(shader);
        return shader;
    }
    
    Ref<Shader> ShaderLibrary::Load(const std::string& filePath, const std::string& name)
    {
        auto shader = CreateRef(Shader::Create(filePath));
        Add(shader, name);
        return shader;
    }
    
    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        PKE_CORE_ASSERT(m_Shaders.contains(name), "Shader with name '{}' was not found in the library.", name);
        
        return m_Shaders[name];
    }
}
