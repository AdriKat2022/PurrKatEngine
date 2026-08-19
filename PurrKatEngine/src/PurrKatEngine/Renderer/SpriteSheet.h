#pragma once
#include "Tex2D.h"
#include "Texture.h"

namespace PurrKatEngine
{
    class SpriteSheet
    {
    public:
        SpriteSheet(const std::string& sheetPath, const Tex2D::SpriteSheetOptions& options = { .CellSize = {32, 32} }) : m_SpriteSheetOptions(options) { m_Texture = Texture2D::CreateRef(sheetPath); }
        SpriteSheet(const Texture2D* texture, const Tex2D::SpriteSheetOptions& options = { .CellSize = {32, 32} }) : m_Texture(texture), m_SpriteSheetOptions(options) {}
        SpriteSheet(const Ref<const Texture2D>& texture, const Tex2D::SpriteSheetOptions& options = { .CellSize = {32, 32} }) : m_Texture(texture), m_SpriteSheetOptions(options) {}
        
        std::vector<Tex2D> GetSpriteArray();
        Tex2D::SpriteSheetOptions& GetSpriteSheetOptions() { return m_SpriteSheetOptions; }
        
        const Ref<const Texture2D>& GetTexture() const { return m_Texture; }
        const glm::ivec2& GetSpriteCount() const { return m_SpriteCount; }
        Tex2D GetSprite(const glm::vec2& coords, const glm::vec2& spriteSize = {1, 1}) const;
        
    private:
        Ref<const Texture2D> m_Texture;
        Tex2D::SpriteSheetOptions m_SpriteSheetOptions;
        glm::ivec2 m_SpriteCount;
    };
}
