#include "pkepch.h"
#include "Renderer2D.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>
#include "Buffer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace PurrKatEngine
{
    struct Renderer2DData
    {
        Scope<VertexArray> QuadVertexArray;
        Scope<Shader> FlatColorShader;
        Scope<Shader> TextureColoredShader;
        
        // glm::mat4 QuadVertexPositions[4];
    };
    
    static Renderer2DData* s_RendererData;
    
    void Renderer2D::Init()
    {
        PKE_CORE_ASSERT(!s_RendererData, "Renderer2D already initialized!");
        
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
        
        s_RendererData->FlatColorShader = ToScope(Shader::Create("assets/shaders/FlatColor.glsl"));
        s_RendererData->TextureColoredShader = ToScope(Shader::Create("assets/shaders/Texture.glsl"));
        s_RendererData->TextureColoredShader->Bind();
        s_RendererData->TextureColoredShader->SetUniformInt("u_Texture", 0);
    }
    
    void Renderer2D::Shutdown()
    {
        delete s_RendererData;
    }
    
    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_RendererData->FlatColorShader->Bind();
        s_RendererData->FlatColorShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        
        s_RendererData->TextureColoredShader->Bind();
        s_RendererData->TextureColoredShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }
    
    void Renderer2D::EndScene()
    {
        // s_RendererData->FlatColorShader->Unbind();
    }
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        // The following bind is safer but costs more performances (if we're, for example, drawing smth in 3D before drawing back in 2D, the wrong shader could be bind).
        s_RendererData->FlatColorShader->Bind();
        
        auto transformMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        s_RendererData->FlatColorShader->SetUniformMat4("u_Transform", transformMatrix);
        s_RendererData->FlatColorShader->SetUniformFloat4("u_Color", color);
        s_RendererData->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_RendererData->QuadVertexArray.get());
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture* texture, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, color);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Texture* texture, const glm::vec4& color)
    {
        // The following bind is safer but costs more performances (if we're, for example, drawing smth in 3D before drawing back in 2D, the wrong shader could be bind).
        s_RendererData->TextureColoredShader->Bind();
        
        auto transformMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        s_RendererData->TextureColoredShader->SetUniformMat4("u_Transform", transformMatrix);
        s_RendererData->TextureColoredShader->SetUniformFloat4("u_Color", color);
        
        texture->Bind();
        
        s_RendererData->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_RendererData->QuadVertexArray.get());
    }
}
