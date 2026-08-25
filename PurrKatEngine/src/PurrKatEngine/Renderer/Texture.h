#pragma once

#include "Tex2D.h"

// The following macros offer a quick way to get the width and height vec while specifying one axis and adjust the other according to its aspect ratio.
#define SET_WIDTH_GEN(texture, width) { width, (width)/(texture)->GetAspectRatio() }
#define SET_HEIGHT_GEN(texture, height) { (height) * (texture)->GetAspectRatio(), height }

#define SET_WIDTH(texture, width) SetWidthWithAspectRatio(texture, width)
#define SET_HEIGHT(texture, height) SetWidthWithAspectRatio(texture, height)

namespace PurrKatEngine
{
    inline glm::vec2 SetWidthWithAspectRatio(const Tex2D& texture, float width)
    {
        return {
            width,
            width / texture.GetAspectRatio()
        };
    }

    inline glm::vec2 SetHeightWithAspectRatio(const Tex2D& texture, float height)
    {
        return {
            height * texture.GetAspectRatio(),
            height
        };
    }
    
    class Texture
    {
    public:
        virtual ~Texture() = default;
        
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        
        virtual uint32_t GetRendererID() const = 0;
        
        virtual float GetAspectRatio() const { return (float)GetWidth() / (float)GetHeight(); }
        
        virtual void SetData(void* data, uint32_t size) = 0;
        
        virtual void Bind(uint32_t slot = 0) const = 0;
        
        virtual bool operator==(const Texture& other) const = 0;
    };
    
    class Texture2D : public Texture
    {
    public:
        enum class FilterType : unsigned char { Linear, Nearest };
        
        struct TextureOptions
        {
            FilterType Filter = FilterType::Linear;
        };
        
    public:
        static Texture2D* Create(uint32_t width, uint32_t height, const TextureOptions& textureOptions = {});
        static Texture2D* Create(const std::string& path, const TextureOptions& textureOptions = {});
        
        static Ref<Texture2D> CreateRef(uint32_t width, uint32_t height, const TextureOptions& textureOptions = {});
        static Ref<Texture2D> CreateRef(const std::string& path, const TextureOptions& textureOptions = {});
    };
}
