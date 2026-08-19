#include "pkepch.h"
#include "Tex2D.h"
#include "Texture.h"

namespace PurrKatEngine
{
    Tex2D::Tex2D()
    {}

    Tex2D::Tex2D(const Ref<const Texture2D>& texture)
        : m_Texture(texture)
    {}

    Tex2D::Tex2D(const Ref<const Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) 
        : m_Texture(texture)
    {
        m_TexCoords[0] = { min.x, min.y };
        m_TexCoords[1] = { max.x, min.y };
        m_TexCoords[2] = { max.x, max.y };
        m_TexCoords[3] = { min.x, max.y };
    }

    Tex2D::Tex2D(const Texture2D* texture)
        : m_Texture(texture)
    {}

    Tex2D::Tex2D(const Texture2D* texture, const glm::vec2& min, const glm::vec2& max)
        : m_Texture(texture)
    {
        m_TexCoords[0] = { min.x, min.y };
        m_TexCoords[1] = { max.x, min.y };
        m_TexCoords[2] = { max.x, max.y };
        m_TexCoords[3] = { min.x, max.y };
    }

    float Tex2D::GetAspectRatio() const
    {
        const float uvWidth  = m_TexCoords[1].x - m_TexCoords[0].x;
        const float uvHeight = m_TexCoords[2].y - m_TexCoords[0].y;

        if (m_Texture == nullptr)
            return uvWidth / uvHeight;
        else
            return m_Texture->GetAspectRatio() * (uvWidth / uvHeight);
    }

    Tex2D Tex2D::CreateFromCoords(const Ref<const Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize)
    {
        const float width = (float)texture->GetWidth();
        const float height = (float)texture->GetHeight();
        
        glm::vec2 min = { coords.x * spriteSize.x / width, coords.y * spriteSize.y / height };
        glm::vec2 max = { (coords.x + 1) * spriteSize.x / width, (coords.y + 1) * spriteSize.y / height };
        
        return {texture, min, max};
    }
}
