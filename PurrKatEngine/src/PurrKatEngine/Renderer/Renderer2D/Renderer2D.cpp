#include "pkepch.h"
#include "Renderer2D.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>
#include "PurrKatEngine/Editor/EditorCamera.h"
#include "PurrKatEngine/Profiling/Profiler.h"
#include "PurrKatEngine/Renderer/Buffer.h"
#include "PurrKatEngine/Renderer/Camera.h"
#include "PurrKatEngine/Renderer/RenderCommand.h"
#include "PurrKatEngine/Renderer/Shader.h"
#include "PurrKatEngine/Renderer/Tex2D.h"
#include "PurrKatEngine/Renderer/Texture.h"
#include "PurrKatEngine/Renderer/VertexArray.h"
#include "PurrKatEngine/Utility/ImGuiUtility.h"

namespace PurrKatEngine
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        glm::vec2 UVTiling;
        float TexIndex;
        
        // Editor Only
        int EntityID = -1; // No entity by default
    };
    
    struct DrawCallData
    {
        uint32_t PersistenceTTL; // If reaches zero, the memory gets freed.
        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        void ResetCountAndPtr()
        {
            QuadVertexBufferPtr = QuadVertexBufferBase;
            QuadIndexCount = 0;
        }
    };
    
    struct Renderer2DData
    {
        static constexpr int POINT_COUNT_PER_QUAD = 4; // Number of different coordinates needed to draw a quad (1 quad = 4 points).
        static constexpr int INDICES_COUNT_PER_QUAD = 6; // Number of indices needed to draw a quad (2 triangles = 6 points).
        
        static constexpr uint32_t BUFFER_CAPACITY_PERSISTENCE = 144; // Frames until a draw call's allocated memory gets freed if unused.
        static constexpr uint32_t MAX_DRAW_CALLS = 1000;
        static constexpr uint32_t MAX_QUADS = 5000;
        static constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
        static constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;
        static constexpr uint32_t MAX_TEXTURE_SLOTS = 32;
        static constexpr uint32_t MAX_LIGHT_COUNT = 16;
        
        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        
        uint32_t DrawCallsCount;
        uint32_t DrawCallsCapacity;
        DrawCallData* DrawCalls = nullptr; // All different draw calls that will occur.
        
        Scope<Shader> SpriteColorShaderEditor;
        Scope<Shader> SpriteColorShader;
        Scope<Shader> SpriteColorShaderLit;
        
        std::vector<LightSource2D> LightSources;
        
        std::array<Ref<const Texture2D>, MAX_TEXTURE_SLOTS> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = White texture
        
        glm::vec4 QuadVertexPositions[4];
        
        Renderer2D::Statistics Stats;
        
        bool IsLitScene;
        
        bool IsEntityIdTemporary = false;
        int CurrentEntityID = -1; // No entity by default
    };
    
    static Renderer2DData s_RendererData;
    
    void Renderer2D::Init()
    {
        s_RendererData.DrawCallsCount = 0;
        s_RendererData.DrawCallsCapacity = 0;
        s_RendererData.DrawCalls = new DrawCallData[Renderer2DData::MAX_DRAW_CALLS];
        
        s_RendererData.QuadVertexBuffer = CreateRef(VertexBuffer::Create(Renderer2DData::MAX_VERTICES * sizeof(QuadVertex)));
        s_RendererData.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float2, "a_UVTiling" },
            { ShaderDataType::Float, "a_TexIndex" },
            { ShaderDataType::Int, "a_EntityId" },
        });
        s_RendererData.QuadVertexArray = CreateRef(VertexArray::Create());
        s_RendererData.QuadVertexArray->AddVertexBuffer(s_RendererData.QuadVertexBuffer);
        
        uint32_t* quadIndices = new uint32_t[Renderer2DData::MAX_INDICES];
        
        uint32_t offset = 0;
        for (uint32_t i = 0; i < Renderer2DData::MAX_INDICES; i += Renderer2DData::INDICES_COUNT_PER_QUAD)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            
            offset += Renderer2DData::POINT_COUNT_PER_QUAD;
        }
        
        Ref<IndexBuffer> quadIB = CreateRef(IndexBuffer::Create(quadIndices, Renderer2DData::MAX_INDICES));
        s_RendererData.QuadVertexArray->SetIndexBuffer(quadIB);
        
        delete[] quadIndices;
        
        uint32_t whitePixel = 0xffffffff;
        Ref<Texture2D> blankTexture = CreateRef(Texture2D::Create(1, 1));
        blankTexture->SetData(&whitePixel, sizeof(whitePixel));
        
        int samplers[Renderer2DData::MAX_TEXTURE_SLOTS];
        for (uint32_t i = 0; i < Renderer2DData::MAX_TEXTURE_SLOTS; i++)
        {
            samplers[i] = (int)i;
        }
        
        s_RendererData.SpriteColorShaderEditor = CreateScope(Shader::Create("assets/shaders/EditorTexture.glsl"));
        s_RendererData.SpriteColorShaderEditor->Bind();
        s_RendererData.SpriteColorShaderEditor->SetUniformIntArray("u_Textures", samplers, Renderer2DData::MAX_TEXTURE_SLOTS);
        
        s_RendererData.SpriteColorShader = CreateScope(Shader::Create("assets/shaders/Texture.glsl"));
        s_RendererData.SpriteColorShader->Bind();
        s_RendererData.SpriteColorShader->SetUniformIntArray("u_Textures", samplers, Renderer2DData::MAX_TEXTURE_SLOTS);

        s_RendererData.SpriteColorShaderLit = CreateScope(Shader::Create("assets/shaders/TextureLit.glsl"));
        s_RendererData.SpriteColorShaderLit->Bind();
        s_RendererData.SpriteColorShaderLit->SetUniformIntArray("u_Textures", samplers, Renderer2DData::MAX_TEXTURE_SLOTS);
        
        s_RendererData.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        s_RendererData.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        s_RendererData.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        s_RendererData.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
        
        s_RendererData.TextureSlots[0] = blankTexture;
        s_RendererData.TextureSlotIndex = 1;
    }
    
    void Renderer2D::Shutdown()
    {
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform, bool litScene)
    {
        s_RendererData.IsLitScene = litScene;
    
        auto projectionViewMatrix = camera.GetProjectionMatrix() * glm::inverse(transform);
        
        if (litScene)
        {
            s_RendererData.SpriteColorShaderLit->Bind();
            s_RendererData.SpriteColorShaderLit->SetUniformMat4("u_ViewProjection", projectionViewMatrix);
        }
        else
        {
            s_RendererData.SpriteColorShader->Bind();
            s_RendererData.SpriteColorShader->SetUniformMat4("u_ViewProjection", projectionViewMatrix);
        }
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera, bool litScene)
    {
        s_RendererData.IsLitScene = litScene;
        
        if (litScene)
        {
            s_RendererData.SpriteColorShaderLit->Bind();
            s_RendererData.SpriteColorShaderLit->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        }
        else
        {
            s_RendererData.SpriteColorShader->Bind();
            s_RendererData.SpriteColorShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        }
    }

    void Renderer2D::BeginScene(const EditorCamera& camera, bool litScene)
    {
        s_RendererData.IsLitScene = litScene;
        
        // if (litScene)
        // {
        //     s_RendererData.SpriteColorShaderLit->Bind();
        //     s_RendererData.SpriteColorShaderLit->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        // }
        // else
        {
            s_RendererData.SpriteColorShaderEditor->Bind();
            s_RendererData.SpriteColorShaderEditor->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        }
    }

    void Renderer2D::EndScene()
    {
        FlushScene();
    }

    void Renderer2D::FlushScene()
    {
        if (s_RendererData.DrawCallsCount != 0)
        {
            if (s_RendererData.IsLitScene)
                UploadLights();
            
            PassDrawCalls();
        }
        
        FreeUnusedBuffers();
        ClearLightSources();
        
        s_RendererData.DrawCallsCount = 0;
        
        s_RendererData.TextureSlotIndex = 1;
    }

    const Renderer2D::Statistics& Renderer2D::GetStatistics()
    {
        return s_RendererData.Stats;
    }

    void Renderer2D::EndFrameStatistics()
    {
        s_RendererData.Stats.DrawCalls = 0;
        s_RendererData.Stats.QuadCount = 0;
    }
    
    // ################## DRAW FUNCTIONS ##################

    void Renderer2D::SetEntityID(int entityID)
    {
        s_RendererData.IsEntityIdTemporary = false;
        s_RendererData.CurrentEntityID = entityID;
    }

    void Renderer2D::SetNextEntityID(int entityID)
    {
        s_RendererData.IsEntityIdTemporary = true;
        s_RendererData.CurrentEntityID = entityID;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const SpriteComponent& spriteComponent)
    {
        DrawQuadInternal(transform, spriteComponent.Texture, spriteComponent.UVTiling, spriteComponent.Color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        const glm::mat4 transform = Transform::CalculateTransformMatrix2D({position.x, position.y, 0}, size);
        DrawQuadInternal(transform, nullptr, {1, 1}, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        const glm::mat4 transform = Transform::CalculateTransformMatrix2D(position, size);
        DrawQuadInternal(transform, nullptr, {1, 1}, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Tex2D& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor)
    {
        const glm::mat4 transform = Transform::CalculateTransformMatrix2D({position.x, position.y, 0}, size);
        DrawQuadInternal(transform, texture, uvTiling, tintColor);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Tex2D& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor)
    {
        const glm::mat4 transform = Transform::CalculateTransformMatrix2D(position, size);
        DrawQuadInternal(transform, texture, uvTiling, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Tex2D& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor)
    {
        DrawQuadInternal(transform, texture, uvTiling, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Tex2D& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor)
    {
        DrawQuadRotatedInternal({ position.x, position.y, 0}, size, rotation, texture, uvTiling, tintColor);
    }
    
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Tex2D& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor)
    {
        DrawQuadRotatedInternal(position, size, rotation, texture, uvTiling, tintColor);
    }

    // ################## LIGHTNING FUNCTIONS ##################
    
    void Renderer2D::AddLightSource(const LightSource2D& lightSource)
    {
        if (s_RendererData.IsLitScene)
            s_RendererData.LightSources.push_back(lightSource);
    }
    
    void Renderer2D::ClearLightSources()
    {
        s_RendererData.LightSources.clear();
    }
    
    // ################## UTILITY FUNCTIONS ####################
    
    void Renderer2D::DrawQuadInternal(const glm::mat4& transform, const Tex2D& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor)
    {
        float textureIndex = GetOrCreateTextureIndex(texture);

        WriteToVertexBuffer(tintColor, transform, textureIndex, uvTiling, texture.GetTexCoords());
    }
    
    void Renderer2D::DrawQuadRotatedInternal(const glm::vec3& position, const glm::vec2& size, float rotation, const Tex2D& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor)
    {
        float textureIndex = GetOrCreateTextureIndex(texture);
        
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        WriteToVertexBuffer(tintColor, transform, textureIndex, uvTiling, texture.GetTexCoords());
    }
    
    void Renderer2D::UploadLights()
    {
        int lightCount = 0;
        Shader& shader = *s_RendererData.SpriteColorShaderLit;
        for (const LightSource2D& light : s_RendererData.LightSources)
        {
            if (light.Radius <= 0.0f || light.Intensity <= 0.0f) continue;

            const std::string uniform = "u_Lights[" + std::to_string(lightCount) + "]";
            shader.SetUniformFloat2(uniform + ".Position", light.Position);
            shader.SetUniformFloat3(uniform + ".Color", light.Color);
            shader.SetUniformFloat(uniform + ".Radius", light.Radius);
            shader.SetUniformFloat(uniform + ".Intensity", light.Intensity);
            lightCount++;
        }

        shader.SetUniformInt("u_LightCount", lightCount);
    }
    
    void Renderer2D::PassDrawCalls()
    {
        // Prepare textures.
        for (uint32_t i = 0; i < s_RendererData.TextureSlotIndex; i++)
        {
            s_RendererData.TextureSlots[i]->Bind(i);
        }
        
        for (uint32_t drawCallIndex = 0; drawCallIndex < s_RendererData.DrawCallsCount; drawCallIndex++)
        {
            const DrawCallData& drawCallData = s_RendererData.DrawCalls[drawCallIndex];
            
            uint32_t dataSize = (uint32_t)((uint8_t*)drawCallData.QuadVertexBufferPtr - (uint8_t*)drawCallData.QuadVertexBufferBase);
            s_RendererData.QuadVertexBuffer->SetData(drawCallData.QuadVertexBufferBase, dataSize);
            RenderCommand::DrawIndexed(s_RendererData.QuadVertexArray.get(), drawCallData.QuadIndexCount);
        }
        
        s_RendererData.Stats.DrawCalls += s_RendererData.DrawCallsCount;
    }
    
    void Renderer2D::FreeUnusedBuffers()
    {
        auto start = s_RendererData.DrawCalls + (s_RendererData.DrawCallsCapacity - 1);
        auto end = s_RendererData.DrawCalls + (s_RendererData.DrawCallsCount - 1);
        
        for (DrawCallData* drawCallDataPtr = start; drawCallDataPtr > end; drawCallDataPtr--)
        {
            if (drawCallDataPtr->PersistenceTTL == 0)
            {
                delete drawCallDataPtr->QuadVertexBufferBase;
                s_RendererData.DrawCallsCapacity--;
            }
            drawCallDataPtr->PersistenceTTL--;
        }
    }

    void Renderer2D::IncreaseDrawCallMemoryIfNeeded(int countToFit)
    {
        if (s_RendererData.DrawCallsCount == 0 ||
            s_RendererData.DrawCalls[s_RendererData.DrawCallsCount-1].QuadIndexCount + countToFit > Renderer2DData::MAX_INDICES)
        {
            // Create a new draw call.
            if (s_RendererData.DrawCallsCount == s_RendererData.DrawCallsCapacity)
            {
                // Allocate for the new draw call.
                
                if (s_RendererData.DrawCallsCapacity == Renderer2DData::MAX_DRAW_CALLS)
                {
                    PKE_CORE_ERROR("Renderer2D: Maximum draw calls reached! Cannot allocate more memory for new draw calls.");
                    throw std::runtime_error("Renderer2D: Maximum draw calls reached!");
                }
                
                auto quadVertexAllocation = new QuadVertex[Renderer2DData::MAX_VERTICES];
                s_RendererData.DrawCalls[s_RendererData.DrawCallsCount].QuadVertexBufferBase = quadVertexAllocation;
                s_RendererData.DrawCallsCapacity++;
            }
            s_RendererData.DrawCalls[s_RendererData.DrawCallsCount].ResetCountAndPtr();
            s_RendererData.DrawCalls[s_RendererData.DrawCallsCount].PersistenceTTL = Renderer2DData::BUFFER_CAPACITY_PERSISTENCE;
            s_RendererData.DrawCallsCount++;
        }
    }

    void Renderer2D::WriteToVertexBuffer(const glm::vec4& color, const glm::mat4& transform, float textureIndex, const glm::vec2& uvTiling, const glm::vec2* texCoords)
    {
        IncreaseDrawCallMemoryIfNeeded(Renderer2DData::INDICES_COUNT_PER_QUAD);
     
        auto& drawCallData = s_RendererData.DrawCalls[s_RendererData.DrawCallsCount-1];
        
        for (int i = 0; i < Renderer2DData::POINT_COUNT_PER_QUAD; ++i)
        {
            drawCallData.QuadVertexBufferPtr->Position = transform * s_RendererData.QuadVertexPositions[i];
            drawCallData.QuadVertexBufferPtr->Color = color;
            drawCallData.QuadVertexBufferPtr->TexCoord = texCoords[i];
            drawCallData.QuadVertexBufferPtr->UVTiling = uvTiling;
            drawCallData.QuadVertexBufferPtr->TexIndex = textureIndex;
#if true
            drawCallData.QuadVertexBufferPtr->EntityID = s_RendererData.CurrentEntityID; // Editor Only
            drawCallData.QuadVertexBufferPtr++;
#endif
        }
        
#if true
        // Editor Only
        if (s_RendererData.IsEntityIdTemporary)
        {
            s_RendererData.CurrentEntityID = -1; // Reset to no entity.
            s_RendererData.IsEntityIdTemporary = false;
        }
#endif
        
        drawCallData.QuadIndexCount += Renderer2DData::INDICES_COUNT_PER_QUAD;
        s_RendererData.Stats.QuadCount++;
    }
    
    float Renderer2D::GetOrCreateTextureIndex(const Tex2D& texture)
    {
        const Ref<const Texture2D>& texture2D = texture.GetTexture();
        
        if (texture2D == nullptr)
        {
            return 0; // Return the white texture.
        }
        
        float textureIndex = -1;
        
        // Fetch the texture index if it already exists.
        for (uint32_t i = 0; i < s_RendererData.TextureSlotIndex; i++)
        {
            auto currentTextureSlot = s_RendererData.TextureSlots[i];
            
            if (currentTextureSlot == nullptr)
            {
                PKE_CORE_WARN("{}: current is NULL", i);
                continue;
            }
            
            if (currentTextureSlot == texture2D)
            {
                textureIndex = (float)i;
                break;
            }
        }
        
        // If not, store it for the current batch.
        if (textureIndex < 0)
        {
            textureIndex = (float)s_RendererData.TextureSlotIndex;
            s_RendererData.TextureSlots[s_RendererData.TextureSlotIndex] = texture2D;
            s_RendererData.TextureSlotIndex++;
        }
        
        return textureIndex;
    }
}
