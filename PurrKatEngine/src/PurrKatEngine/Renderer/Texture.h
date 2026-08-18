#pragma once

// The following macros offer a quick way to get the width and height vec while specifying one axis and adjust the other according to its aspect ratio.
#define SET_WIDTH(texture, width) { width, width/texture->GetAspectRatio() }
#define SET_HEIGHT(texture, height) { height*texture->GetAspectRatio(), height }

namespace PurrKatEngine
{
    class Texture
    {
    public:
        virtual ~Texture() = default;
        
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        
        virtual float GetAspectRatio() { return (float)GetWidth() / (float)GetHeight(); }
        
        virtual void SetData(void* data, uint32_t size) = 0;
        
        virtual void Bind(uint32_t slot = 0) const = 0;
        
        virtual bool operator==(const Texture& other) const = 0;
    };
    
    class Texture2D : public Texture
    {
    public:
        enum class FilterType : byte { Linear, Nearest };
        
        struct TextureOptions
        {
            FilterType Filter = FilterType::Linear;
        };
        
    public:
        static Texture2D* Create(uint32_t width, uint32_t height);
        static Texture2D* Create(const std::string& path, const TextureOptions& textureOptions = {});
    };
}
