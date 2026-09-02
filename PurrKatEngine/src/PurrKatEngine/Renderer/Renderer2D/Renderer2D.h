#pragma once
#include "PurrKatEngine/Components/Transform.h"
#include "PurrKatEngine/Renderer/OrthographicCamera.h"
#include "PurrKatEngine/Renderer/Tex2D.h"
#include "PurrKatEngine/Scene/Components.h"

namespace PurrKatEngine
{
    class EditorCamera;
    class Camera;

    struct LightSource2D
    {
        glm::vec2 Position{0.0f};
        glm::vec3 Color{1.0f};
        float Radius = 1.0f;
        float Intensity = 1.0f;
    };

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const Camera& camera, const glm::mat4& transform, bool litScene = false);
        static void BeginScene(const OrthographicCamera& camera, bool litScene = false);
        static void BeginScene(const EditorCamera& camera, bool litScene = false);
        static void EndScene();
        static void FlushScene();

        struct Statistics
        {
            uint32_t DrawCalls;
            uint32_t QuadCount;
            
            uint32_t GetVertexCount() const { return QuadCount * 4; }
            uint32_t GetIndexCount() const { return QuadCount * 6; }
        };
        
        static const Statistics& GetStatistics();
        static void EndFrameStatistics();
        
        struct DrawOptions
        {
            glm::vec3 Position = {0.0f, 0.0f, 0.0f};
            glm::vec2 Size = {1.0f, 1.0f};
            
            float Rotation = 0;
            
            Tex2D Texture{};
            glm::vec2 UVTiling = {1.0f, 1.0f};
            glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
        };
        
        // ########### DRAW FUNCTIONS ############
        
        static void SetNextEntityID(int entityID = -1);
        static void SetEntityID(int entityID = -1);
        
        static void DrawQuad(const glm::mat4& transform, const SpriteComponent& spriteComponent);
        
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
        
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Tex2D& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Tex2D& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        static void DrawQuad(const glm::mat4& transform, const Tex2D& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor);
        
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Tex2D& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Tex2D& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        
        
        // ########## LIGHTING ###########
        
        static void AddLightSource(const LightSource2D& lightSource);
        static void ClearLightSources();
        
    private:
        static void DrawQuadInternal(const glm::mat4& transform, const Tex2D& texture, const glm::vec2& uvTiling, const glm::vec4& tintColor);
        static void DrawQuadRotatedInternal(const glm::vec3& position, const glm::vec2& size, float rotation, const Tex2D& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});

        static void UploadLights();
        static void PassDrawCalls();
        static void FreeUnusedBuffers();
        static void IncreaseDrawCallMemoryIfNeeded(int countToFit);
        static void WriteToVertexBuffer(const glm::vec4& color, const glm::mat4& transform, float textureIndex, const glm::vec2& uvTiling, const glm::vec2* texCoords);
        static float GetOrCreateTextureIndex(const Tex2D& texture);
    };
}

