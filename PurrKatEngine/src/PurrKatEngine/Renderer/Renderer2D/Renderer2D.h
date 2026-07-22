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
        
        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void FlushScene();
        
        // Primitives
        
        // ########### UNLIT ############
        
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
        
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<const Texture2D>& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<const Texture2D>& texture, const glm::vec2& uvTiling = {1, 1}, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        
        // ########### LIT ############
        
        static void DrawLitQuad( const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, float ambientStrength = 0.15f);
        static void DrawLitQuad( const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, float ambientStrength = 0.15f);
        
        static void DrawLitQuad(const glm::vec2& position, const glm::vec2& size, const Texture2D* texture, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f}, float ambientStrength = 0.15f, const glm::vec2& uvTiling = {1.0f, 1.0f});
        static void DrawLitQuad(const glm::vec3& position, const glm::vec2& size, const Texture2D* texture, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f}, float ambientStrength = 0.15f, const glm::vec2& uvTiling = {1.0f, 1.0f});
        
        // ########## LIGHTING ###########
        
        static void ClearLightSources();
        static void AddLightSource(const LightSource2D& lightSource);
    };
}

