#include "pkepch.h"
#include "SpriteSheet.h"

#include "Texture.h"
#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    Tex2D SpriteSheet::GetSprite(const glm::vec2& coords, const glm::vec2& spriteSize) const
    {
        return Tex2D::CreateFromCoords(m_Texture, coords, m_SpriteSheetOptions, spriteSize);
    }

    std::vector<Tex2D> SpriteSheet::GetSpriteArray()
    {
        std::vector<Tex2D> spriteArray;
        
        glm::ivec2 cellCount{m_SpriteSheetOptions.CellCount.x, m_SpriteSheetOptions.CellCount.y};
        
        if (cellCount.x <= 0 || cellCount.y <= 0)
        {
            glm::ivec2 cellSize = m_SpriteSheetOptions.CellSize;
            
            if (cellSize.x <= 0 || cellSize.y <= 0)
            {
                PKE_CORE_ERROR("SpriteSheet: Either CellCount or CellSize must be specified.");
                return {};
            }
            
            // Use the cell size to compute the cell count.
 
            if (cellSize.x + m_SpriteSheetOptions.Padding.x == 0 || cellSize.y + m_SpriteSheetOptions.Padding.y == 0)
            {
                PKE_CORE_WARN("Padding and Cell size cancel such as it's impossible to parse the sprite sheet.");
                return {};
            }
            
            uint32_t current = 0;
            while (current < m_Texture->GetWidth())
            {
                cellCount.x++;
                current += (uint32_t)(cellSize.x + m_SpriteSheetOptions.Padding.x);
            }
            current = 0;
            while (current < m_Texture->GetHeight())
            {
                cellCount.y++;
                current += (uint32_t)(cellSize.y + m_SpriteSheetOptions.Padding.y);
            }
        }
        
        // Use the cell count.
        for (int i = 0; i < cellCount.x; i++)
        {
            for (int j = 0; j < cellCount.y; j++)
            {
                spriteArray.push_back(GetSprite({i, j}));
            }
        }
        
        m_SpriteCount = {cellCount.x, cellCount.y};
        
        return spriteArray;
    }
}
