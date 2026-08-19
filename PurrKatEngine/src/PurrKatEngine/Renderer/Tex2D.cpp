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

    Tex2D Tex2D::CreateFromCoords(const Ref<const Texture2D>& texture, const glm::vec2& coords, const SpriteSheetOptions& options, const glm::vec2& spriteSize)
    {
        const float width = (float)texture->GetWidth();
        const float height = (float)texture->GetHeight();

        glm::vec2 padding = options.Padding;
        glm::vec2 cellSize = options.CellSize;
        
        if (options.CellCount.x > 0 && options.CellCount.y > 0)
        {
            glm::vec2 cellCount = options.CellCount;
            glm::vec2 totalPadding = { padding.x*(cellCount.x-1), padding.y*(cellCount.y-1)};
            cellSize = { (width-totalPadding.x)/cellCount.x, (height-totalPadding.y)/cellCount.y };
        }

        glm::vec2 rightPadding = { (coords.x + spriteSize.x - 1) * padding.x / width, (coords.y + spriteSize.y - 1) * padding.y / height };
        
        glm::vec2 min = { 
            coords.x * (cellSize.x + padding.x) / width, 
            coords.y * (cellSize.y + padding.y) / height
        };
        
        glm::vec2 max = {
            (coords.x + spriteSize.x) * cellSize.x / width + rightPadding.x,
            (coords.y + spriteSize.y) * cellSize.y / height + rightPadding.y
        };
        
        return {texture, min, max};
    }
}
