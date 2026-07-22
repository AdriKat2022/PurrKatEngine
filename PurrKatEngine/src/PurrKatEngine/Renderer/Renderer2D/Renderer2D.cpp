#include "pkepch.h"
#include "Renderer2D.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>
#include "PurrKatEngine/Profiling/Profiler.h"
#include "PurrKatEngine/Renderer/Buffer.h"
#include "PurrKatEngine/Renderer/RenderCommand.h"
#include "PurrKatEngine/Renderer/Shader.h"
#include "PurrKatEngine/Renderer/VertexArray.h"

namespace PurrKatEngine
{
    constexpr int MAX_LIGHT_COUNT = 16;
    
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        glm::vec2 UVTiling;
        float TexIndex;
    };
    
    struct Renderer2DData
    {
    
        // Per draw call, the following is the maximum.
        const uint32_t MAX_QUADS = 10000;
        const uint32_t MAX_VERTICES = MAX_QUADS * 4;
        const uint32_t MAX_INDICES = MAX_QUADS * 6;
        static constexpr uint32_t MAX_TEXTURE_SLOTS = 32;
        
        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        
        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;
        
        Scope<Shader> SpriteColorShader;
        Scope<Shader> SpriteColorShaderLit;
        
        std::vector<LightSource2D> LightSources;
        
        std::array<Ref<const Texture2D>, MAX_TEXTURE_SLOTS> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = White texture
    };
    
    static Renderer2DData s_RendererData;
    
    
    void Renderer2D::Init()
    {
        s_RendererData.QuadVertexArray = ToRef(VertexArray::Create());

        s_RendererData.QuadVertexBuffer = ToRef(VertexBuffer::Create(s_RendererData.MAX_VERTICES * sizeof(QuadVertex)));
        s_RendererData.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float2, "a_UVTiling" },
            { ShaderDataType::Float, "a_TexIndex" }
        });
        s_RendererData.QuadVertexArray->AddVertexBuffer(s_RendererData.QuadVertexBuffer);
        
        s_RendererData.QuadVertexBufferBase = new QuadVertex[s_RendererData.MAX_VERTICES];
        
        uint32_t* quadIndices = new uint32_t[s_RendererData.MAX_INDICES];
        
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_RendererData.MAX_INDICES; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            
            offset += 4;
        }
        
        Ref<IndexBuffer> quadIB = ToRef(IndexBuffer::Create(quadIndices, s_RendererData.MAX_INDICES));
        s_RendererData.QuadVertexArray->SetIndexBuffer(quadIB);
        
        delete[] quadIndices;
        
        uint32_t whitePixel = 0xffffffff;
        Ref<Texture2D> blankTexture = ToRef(Texture2D::Create(1, 1));
        blankTexture->SetData(&whitePixel, sizeof(whitePixel));
        
        int samplers[Renderer2DData::MAX_TEXTURE_SLOTS];
        for (uint32_t i = 0; i < Renderer2DData::MAX_TEXTURE_SLOTS; i++)
        {
            samplers[i] = (int)i;
        }
        
        s_RendererData.SpriteColorShader = ToScope(Shader::Create("assets/shaders/Texture.glsl"));
        s_RendererData.SpriteColorShader->Bind();
        s_RendererData.SpriteColorShader->SetUniformIntArray("u_Textures", samplers, Renderer2DData::MAX_TEXTURE_SLOTS);

        // s_RendererData.SpriteColorShaderLit = ToScope(Shader::Create("assets/shaders/TextureLit.glsl"));
        // s_RendererData.SpriteColorShaderLit->Bind();
        // s_RendererData.SpriteColorShaderLit->SetUniformInt("u_Texture", 0);
        
        s_RendererData.TextureSlots[0] = blankTexture;
    }
    
    void Renderer2D::Shutdown()
    {
    }
    
    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        // The following is NOT OPTIMIZED since we may not need all the shaders during this scene.
        
        s_RendererData.SpriteColorShader->Bind();
        s_RendererData.SpriteColorShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        
        // s_RendererData.SpriteColorShaderLit->Bind();
        // s_RendererData.SpriteColorShaderLit->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        
        s_RendererData.QuadVertexBufferPtr = s_RendererData.QuadVertexBufferBase;
        s_RendererData.QuadIndexCount = 0;
        
        s_RendererData.TextureSlotIndex = 1;
    }
    
    void Renderer2D::EndScene()
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_RendererData.QuadVertexBufferPtr - (uint8_t*)s_RendererData.QuadVertexBufferBase);
        s_RendererData.QuadVertexBuffer->SetData(s_RendererData.QuadVertexBufferBase, dataSize);
        
        FlushScene();
    }

    void Renderer2D::FlushScene()
    {
        for (uint32_t i = 0; i < s_RendererData.TextureSlotIndex; i++)
        {
            s_RendererData.TextureSlots[i]->Bind(i);
        }
        RenderCommand::DrawIndexed(s_RendererData.QuadVertexArray.get(), s_RendererData.QuadIndexCount);
    }

    // ################## UNLIT FUNCTIONS ##################
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        constexpr float textureIndex = 0;
        constexpr glm::vec2 uvTiling = {1, 1};
        
        s_RendererData.QuadVertexBufferPtr->Position = {position.x - size.x/2, position.y - size.y/2, position.z};
        s_RendererData.QuadVertexBufferPtr->Color = color;
        s_RendererData.QuadVertexBufferPtr->TexCoord = {0, 0};
        s_RendererData.QuadVertexBufferPtr->UVTiling = uvTiling;
        s_RendererData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RendererData.QuadVertexBufferPtr++;
        
        s_RendererData.QuadVertexBufferPtr->Position = {position.x + size.x/2, position.y - size.y/2, position.z};
        s_RendererData.QuadVertexBufferPtr->Color = color;
        s_RendererData.QuadVertexBufferPtr->TexCoord = {1, 0};
        s_RendererData.QuadVertexBufferPtr->UVTiling = uvTiling;
        s_RendererData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RendererData.QuadVertexBufferPtr++;
        
        s_RendererData.QuadVertexBufferPtr->Position = {position.x + size.x/2, position.y + size.y/2, position.z};
        s_RendererData.QuadVertexBufferPtr->Color = color;
        s_RendererData.QuadVertexBufferPtr->TexCoord = {1, 1};
        s_RendererData.QuadVertexBufferPtr->UVTiling = uvTiling;
        s_RendererData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RendererData.QuadVertexBufferPtr++;
        
        s_RendererData.QuadVertexBufferPtr->Position = {position.x - size.x/2, position.y + size.y/2, position.z};
        s_RendererData.QuadVertexBufferPtr->Color = color;
        s_RendererData.QuadVertexBufferPtr->TexCoord = {0, 1};
        s_RendererData.QuadVertexBufferPtr->UVTiling = uvTiling;
        s_RendererData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RendererData.QuadVertexBufferPtr++;
        
        s_RendererData.QuadIndexCount += 6;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<const Texture2D>& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, uvTiling, tintColor);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<const Texture2D>& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor)
    {
        PKE_CORE_ASSERT(texture, "Texture is null!")
        
        glm::vec4 color = tintColor;
        float textureIndex = 0;
        
        // Fetch the texture index if it already exists.
        for (uint32_t i = 0; i < s_RendererData.TextureSlotIndex; i++)
        {
            if (*s_RendererData.TextureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }
        
        // If not, store it for the current batch.
        if (textureIndex == 0)
        {
            textureIndex = (float)s_RendererData.TextureSlotIndex;
            s_RendererData.TextureSlots[s_RendererData.TextureSlotIndex] = texture;
            s_RendererData.TextureSlotIndex++;
        }
        
        s_RendererData.QuadVertexBufferPtr->Position = {position.x - size.x/2, position.y - size.y/2, position.z};
        s_RendererData.QuadVertexBufferPtr->Color = color;
        s_RendererData.QuadVertexBufferPtr->TexCoord = {0, 0};
        s_RendererData.QuadVertexBufferPtr->UVTiling = uvTiling;
        s_RendererData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RendererData.QuadVertexBufferPtr++;
        
        s_RendererData.QuadVertexBufferPtr->Position = {position.x + size.x/2, position.y - size.y/2, position.z};
        s_RendererData.QuadVertexBufferPtr->Color = color;
        s_RendererData.QuadVertexBufferPtr->TexCoord = {1, 0};
        s_RendererData.QuadVertexBufferPtr->UVTiling = uvTiling;
        s_RendererData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RendererData.QuadVertexBufferPtr++;
        
        s_RendererData.QuadVertexBufferPtr->Position = {position.x + size.x/2, position.y + size.y/2, position.z};
        s_RendererData.QuadVertexBufferPtr->Color = color;
        s_RendererData.QuadVertexBufferPtr->TexCoord = {1, 1};
        s_RendererData.QuadVertexBufferPtr->UVTiling = uvTiling;
        s_RendererData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RendererData.QuadVertexBufferPtr++;
        
        s_RendererData.QuadVertexBufferPtr->Position = {position.x - size.x/2, position.y + size.y/2, position.z};
        s_RendererData.QuadVertexBufferPtr->Color = color;
        s_RendererData.QuadVertexBufferPtr->TexCoord = {0, 1};
        s_RendererData.QuadVertexBufferPtr->UVTiling = uvTiling;
        s_RendererData.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_RendererData.QuadVertexBufferPtr++;
        
        s_RendererData.QuadIndexCount += 6;
    }
    
    // ################## LIT FUNCTIONS ##################
    
    void Renderer2D::DrawLitQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float ambientStrength)
    {
        DrawLitQuad({position.x, position.y, 0.0f}, size, color, ambientStrength);
    }

    void Renderer2D::DrawLitQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float ambientStrength)
    {
        Shader& shader = *s_RendererData.SpriteColorShaderLit.get();
        
        shader.Bind();

        const auto transformMatrix = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        shader.SetUniformFloat4("u_Color", color);
        shader.SetUniformMat4("u_Transform", transformMatrix);
        shader.SetUniformFloat("u_AmbientStrength", ambientStrength);

        int lightCount = 0;
        const glm::vec2 quadMin = glm::vec2(position) - glm::abs(size);
        const glm::vec2 quadMax = glm::vec2(position) + glm::abs(size);

        for (const LightSource2D& light : s_RendererData.LightSources)
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

        // s_RendererData.BlankTexture->Bind();
        s_RendererData.QuadVertexArray->Bind();
        
        RenderCommand::DrawIndexed(s_RendererData.QuadVertexArray.get());
    }
    
    void Renderer2D::DrawLitQuad(const glm::vec2& position, const glm::vec2& size, const Texture2D* texture, const glm::vec4& tintColor, float ambientStrength, const glm::vec2& uvTiling)
    {
        DrawLitQuad({position.x, position.y, 0.0f}, size, texture, tintColor, ambientStrength, uvTiling);
    }

    void Renderer2D::DrawLitQuad(const glm::vec3& position, const glm::vec2& size, const Texture2D* texture, const glm::vec4& tintColor, float ambientStrength, const glm::vec2& uvTiling)
    {
        PROFILE_FUNCTION();
        
        Shader& shader = *s_RendererData.SpriteColorShaderLit.get();
        
        shader.Bind();

        const auto transformMatrix = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        shader.SetUniformMat4("u_Transform", transformMatrix);
        shader.SetUniformFloat4("u_Color", tintColor);
        shader.SetUniformFloat2("u_TexScale", uvTiling);
        shader.SetUniformFloat("u_AmbientStrength", ambientStrength);

        int lightCount = 0;
        const glm::vec2 quadMin = glm::vec2(position) - glm::abs(size);
        const glm::vec2 quadMax = glm::vec2(position) + glm::abs(size);

        for (const LightSource2D& light : s_RendererData.LightSources)
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
        s_RendererData.QuadVertexArray->Bind();
        
        RenderCommand::DrawIndexed(s_RendererData.QuadVertexArray.get());
    }
    
    // ################## LIGHTNING FUNCTIONS ##################
    
    void Renderer2D::ClearLightSources()
    {
        s_RendererData.LightSources.clear();
    }
    
    void Renderer2D::AddLightSource(const LightSource2D& lightSource)
    {
        s_RendererData.LightSources.push_back(lightSource);
    }
}
