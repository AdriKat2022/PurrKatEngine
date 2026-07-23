#pragma once
#include "PurrKatEngine/Renderer/OrthographicCamera.h"
#include "PurrKatEngine/Renderer/Texture.h"

namespace PurrKatEngine
{
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
        
        static void BeginScene(const OrthographicCamera& camera, bool litScene = false);
        static void EndScene();
        static void FlushScene();
        static void UploadLights();
        
        struct Statistics
        {
            uint32_t DrawCalls;
            uint32_t QuadCount;
            
            uint32_t GetVertexCount() const { return QuadCount * 4; }
            uint32_t GetIndexCount() const { return QuadCount * 6; }
        };
        
        static const Statistics& GetStatistics();
        static void EndFrameStatistics();
        
        // ########### UNLIT ############
        
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
        
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<const Texture2D>& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<const Texture2D>& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<const Texture2D>& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<const Texture2D>& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        
        // ########### LIT ############
        
        static void DrawLitQuad( const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, float ambientStrength = 0.15f);
        static void DrawLitQuad( const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, float ambientStrength = 0.15f);
        
        static void DrawLitQuad(const glm::vec2& position, const glm::vec2& size, const Ref<const Texture2D>& texture, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f}, float ambientStrength = 0.15f, const glm::vec2& uvTiling = {1.0f, 1.0f});
        static void DrawLitQuad(const glm::vec3& position, const glm::vec2& size, const Ref<const Texture2D>& texture, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f}, float ambientStrength = 0.15f, const glm::vec2& uvTiling = {1.0f, 1.0f});
        
        // ########## LIGHTING ###########
        
        static void AddLightSource(const LightSource2D& lightSource);
        static void ClearLightSources();
        
    private:
        static void WriteToVertexBuffer(const glm::vec4& color, const glm::mat4& transform, const float textureIndex, const glm::vec2 uvTiling);
        static float GetOrCreateTextureIndex(const Ref<const Texture2D>& texture);
        
    };
}

