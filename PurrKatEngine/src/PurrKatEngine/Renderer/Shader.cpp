#include "pkepch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace PurrKatEngine
{
    std::string Shader::s_TextureShaderVertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position*2, 1.0);
}
)";
    
    
    // Shows the passed TexCoord as color.
    std::string Shader::s_TextureShaderFramgmentSrc = R"(
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, v_TexCoord);
}
)";
    
    // Simple vertex source that draws all vertexes according to the camera's placement (ViewProjection) and the object's transform.  
    std::string Shader::s_WorldVertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position*2, 1.0);
}
)";
    
    // Vertex source that draws all vertexes according to the camera's placement (ViewProjection) and the object's transform. Additionally, forwards the custom passed color to the fragment shader.
    std::string Shader::s_WorldCustomColorVertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
out vec4 v_Color;
void main()
{
    v_Color = a_Color;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position*2, 1.0);
}
)";
    
    // Vertex source that draws all vertexes according to the camera's placement (ViewProjection) and the object's transform. Additionally, forwards the position as color to the fragment shader.
    std::string Shader::s_WorldPositionColorVertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
out vec4 v_Color;
void main()
{
    v_Color = vec4(a_Position, 1.0);
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position*2, 1.0);
}
)";

    // Vertex source that draws all vertexes according to the camera's placement (ViewProjection) and the object's transform. Additionally, forwards the pixel screen's position as color to the fragment shader.
    std::string Shader::s_WorldPositionScreenColorVertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
out vec4 v_Color;
void main()
{
    vec4 screenPos = u_ViewProjection * u_Transform * vec4(a_Position*2, 1.0);
    gl_Position = screenPos;
    v_Color = vec4(screenPos);
}
)";
    
    // Fragment source outputing the v_Color property. 
    std::string Shader::s_IdentityFragmentSrc = R"(
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_Color;

void main()
{
    color = v_Color;
}
)";
    
    // Fragment source outputing the u_Color uniform property.
    std::string Shader::s_FlatColorFragmentSrc = R"(
#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_Color;

void main()
{
    color = u_Color;
}
)";
    
    Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSrc)
    {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:
                PKE_CORE_ASSERT(false, "RendererAPI::None is currently not supported.")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertexSource, fragmentSrc);
        }
        
        return nullptr;
    }
    
    Shader* Shader::MakePositionColorShader()
    {
        return Create(s_WorldPositionColorVertexSrc, s_IdentityFragmentSrc);
    }
    
    Shader* Shader::MakeScreenPositionColorShader()
    {
        return Create(s_WorldPositionScreenColorVertexSrc, s_IdentityFragmentSrc);
    }
    
    Shader* Shader::MakeFlatColorShader()
    {
        return Create(s_WorldVertexSrc, s_FlatColorFragmentSrc);
    }

    Shader* Shader::MakeCustomColorShader()
    {
        return Create(s_WorldCustomColorVertexSrc, s_IdentityFragmentSrc);
    }
    
    Shader* Shader::MakeTextureShader()
    {
        return Create(s_TextureShaderVertexSrc, s_TextureShaderFramgmentSrc);
    }
}
