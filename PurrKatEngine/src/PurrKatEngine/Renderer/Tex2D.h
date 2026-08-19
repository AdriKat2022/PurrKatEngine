#pragma once

namespace PurrKatEngine
{
    class Texture2D;

    class Tex2D
    {
    public:
        Tex2D();
        
        Tex2D(const Ref<const Texture2D>& texture);
        Tex2D(const Ref<const Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
        
        Tex2D(const Texture2D* texture);
        Tex2D(const Texture2D* texture, const glm::vec2& min, const glm::vec2& max);

        float GetAspectRatio() const;
        const Ref<const Texture2D>& GetTexture() const { return m_Texture; }
        const glm::vec2* GetTexCoords() const { return m_TexCoords; }
        glm::vec2* GetTexCoordsPtr() { return m_TexCoords; }
        
        static Tex2D CreateFromCoords(const Ref<const Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize);
        
    private:
        Ref<const Texture2D> m_Texture = nullptr;
        glm::vec2 m_TexCoords[4]{ {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
    };
}
