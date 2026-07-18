#include "pkepch.h"
#include "Renderer2D.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>
#include "PurrKatEngine/Renderer/Buffer.h"
#include "PurrKatEngine/Renderer/RenderCommand.h"
#include "PurrKatEngine/Renderer/Shader.h"
#include "PurrKatEngine/Renderer/VertexArray.h"

namespace PurrKatEngine
{
    constexpr int MAX_LIGHT_COUNT = 16;
    
    struct Renderer2DData
    {
        Scope<VertexArray> QuadVertexArray;
        
        Scope<Shader> SpriteColorShader;
        Scope<Shader> SpriteColorShaderLit;
        
        Scope<Texture2D> BlankTexture;
        
        std::vector<LightSource2D> LightSources;
    };
    
    static Renderer2DData* s_RendererData;
    
    void Renderer2D::Init()
    {
        PKE_CORE_ASSERT(!s_RendererData, "Renderer2D already initialized!")
        
        s_RendererData = new Renderer2DData();
        s_RendererData->QuadVertexArray = ToScope(VertexArray::Create());
        
        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        };

        Ref<VertexBuffer> squareVB = ToRef(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        });
        s_RendererData->QuadVertexArray->AddVertexBuffer(squareVB);
        
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Ref<IndexBuffer> squareIB = ToRef(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_RendererData->QuadVertexArray->SetIndexBuffer(squareIB);
        
        uint32_t whitePixel = 0xffffffff;
        s_RendererData->BlankTexture = ToScope(Texture2D::Create(1, 1));
        s_RendererData->BlankTexture->SetData(&whitePixel, sizeof(whitePixel));
        
        s_RendererData->SpriteColorShader = ToScope(Shader::Create("assets/shaders/Texture.glsl"));
        s_RendererData->SpriteColorShader->Bind();
        s_RendererData->SpriteColorShader->SetUniformInt("u_Texture", 0);

        s_RendererData->SpriteColorShaderLit = ToScope(Shader::Create("assets/shaders/TextureLit.glsl"));
        s_RendererData->SpriteColorShaderLit->Bind();
        s_RendererData->SpriteColorShaderLit->SetUniformInt("u_Texture", 0);
    }
    
    void Renderer2D::Shutdown()
    {
        delete s_RendererData;
    }
    
    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        // The following is NOT OPTIMIZED since we may not need all the shaders during this scene.
        
        s_RendererData->SpriteColorShader->Bind();
        s_RendererData->SpriteColorShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        
        s_RendererData->SpriteColorShaderLit->Bind();
        s_RendererData->SpriteColorShaderLit->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }
    
    void Renderer2D::EndScene()
    {
    }
    
    // ################## UNLIT FUNCTIONS ##################
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        // The following bind is safer but costs more performances (if we're, for example, drawing smth in 3D before drawing back in 2D, the wrong shader could be bind).
        s_RendererData->SpriteColorShader->Bind();
        
        auto transformMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        s_RendererData->SpriteColorShader->SetUniformMat4("u_Transform", transformMatrix);
        s_RendererData->SpriteColorShader->SetUniformFloat4("u_Color", color);
        s_RendererData->QuadVertexArray->Bind();
        
        s_RendererData->BlankTexture->Bind();
        
        RenderCommand::DrawIndexed(s_RendererData->QuadVertexArray.get());
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture* texture, const glm::vec2& tilingCount, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingCount, color);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Texture* texture, const glm::vec2& tilingCount, const glm::vec4& color)
    {
        // The following bind is safer but costs more performances (if we're, for example, drawing smth in 3D before drawing back in 2D, the wrong shader could be bind).
        s_RendererData->SpriteColorShader->Bind();
        
        auto transformMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        s_RendererData->SpriteColorShader->SetUniformMat4("u_Transform", transformMatrix);
        s_RendererData->SpriteColorShader->SetUniformFloat4("u_Color", color);
        s_RendererData->SpriteColorShader->SetUniformFloat2("u_TexScale", tilingCount);
        
        texture->Bind();
        
        s_RendererData->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_RendererData->QuadVertexArray.get());
    }
    
    // ################## LIT FUNCTIONS ##################
    
    void Renderer2D::DrawLitQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float ambientStrength)
    {
        DrawLitQuad({position.x, position.y, 0.0f}, size, color, ambientStrength);
    }

    void Renderer2D::DrawLitQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float ambientStrength)
    {
        Shader& shader = *s_RendererData->SpriteColorShaderLit.get();
        
        shader.Bind();

        const auto transformMatrix = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        shader.SetUniformFloat4("u_Color", color);
        shader.SetUniformMat4("u_Transform", transformMatrix);
        shader.SetUniformFloat("u_AmbientStrength", ambientStrength);

        int lightCount = 0;
        const glm::vec2 quadMin = glm::vec2(position) - glm::abs(size);
        const glm::vec2 quadMax = glm::vec2(position) + glm::abs(size);

        for (const LightSource2D& light : s_RendererData->LightSources)
        {
            if (lightCount == MAX_LIGHT_COUNT) break;

            if (light.Radius <= 0.0f || light.Intensity <= 0.0f) continue;

            const glm::vec2 closestPoint = glm::clamp(light.Position, quadMin, quadMax);
            const glm::vec2 offset = light.Position - closestPoint;
            
            if (glm::dot(offset, offset) > light.Radius * light.Radius) continue;

            const std::string uniform = "u_Lights[" + std::to_string(lightCount) + "]";
            shader.SetUniformFloat2(uniform + ".Position", light.Position);
            shader.SetUniformFloat3(uniform + ".Color", light.Color);
            shader.SetUniformFloat(uniform + ".Radius", light.Radius);
            shader.SetUniformFloat(uniform + ".Intensity", light.Intensity);
            ++lightCount;
        }

        shader.SetUniformInt("u_LightCount", lightCount);

        s_RendererData->BlankTexture->Bind();
        s_RendererData->QuadVertexArray->Bind();
        
        RenderCommand::DrawIndexed(s_RendererData->QuadVertexArray.get());
    }
    
    void Renderer2D::DrawLitQuad(const glm::vec2& position, const glm::vec2& size, const Texture* texture, const glm::vec4& color, float ambientStrength, const glm::vec2& tilingCount)
    {
        DrawLitQuad({position.x, position.y, 0.0f}, size, texture, color, ambientStrength, tilingCount);
    }

    void Renderer2D::DrawLitQuad(const glm::vec3& position, const glm::vec2& size, const Texture* texture, const glm::vec4& color, float ambientStrength, const glm::vec2& tilingCount)
    {
        Shader& shader = *s_RendererData->SpriteColorShaderLit.get();
        
        shader.Bind();

        const auto transformMatrix = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        shader.SetUniformMat4("u_Transform", transformMatrix);
        shader.SetUniformFloat4("u_Color", color);
        shader.SetUniformFloat2("u_TexScale", tilingCount);
        shader.SetUniformFloat("u_AmbientStrength", ambientStrength);

        int lightCount = 0;
        const glm::vec2 quadMin = glm::vec2(position) - glm::abs(size);
        const glm::vec2 quadMax = glm::vec2(position) + glm::abs(size);

        for (const LightSource2D& light : s_RendererData->LightSources)
        {
            if (lightCount == MAX_LIGHT_COUNT) break;

            if (light.Radius <= 0.0f || light.Intensity <= 0.0f) continue;

            const glm::vec2 closestPoint = glm::clamp(light.Position, quadMin, quadMax);
            const glm::vec2 offset = light.Position - closestPoint;
            
            if (glm::dot(offset, offset) > light.Radius * light.Radius) continue;

            const std::string uniform = "u_Lights[" + std::to_string(lightCount) + "]";
            shader.SetUniformFloat2(uniform + ".Position", light.Position);
            shader.SetUniformFloat3(uniform + ".Color", light.Color);
            shader.SetUniformFloat(uniform + ".Radius", light.Radius);
            shader.SetUniformFloat(uniform + ".Intensity", light.Intensity);
            ++lightCount;
        }

        shader.SetUniformInt("u_LightCount", lightCount);

        texture->Bind();
        s_RendererData->QuadVertexArray->Bind();
        
        RenderCommand::DrawIndexed(s_RendererData->QuadVertexArray.get());
    }
    
    // ################## LIGHTNING FUNCTIONS ##################
    
    void Renderer2D::ClearLightSources()
    {
        s_RendererData->LightSources.clear();
    }
    
    void Renderer2D::AddLightSource(const LightSource2D& lightSource)
    {
        s_RendererData->LightSources.push_back(lightSource);
    }
}
