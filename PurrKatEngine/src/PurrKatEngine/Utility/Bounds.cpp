#include "pkepch.h"
#include "Bounds.h"

namespace PurrKatEngine
{
    Bounds::Bounds() = default;
    Bounds::Bounds(const glm::vec2& min, const glm::vec2& max): Min(min), Max(max) {}
    Bounds::Bounds(float minX, float minY, float maxX, float maxY): Min(minX, minY), Max(maxX, maxY) {}
    
    std::string Bounds::ToString() const
    {
        std::stringstream ss;
        ss << "Min: [" << Min.x << ", " << Min.y << "], Max: [" << Max.x << ", " << Max.y << "]";
        ss << " Size: [" << GetWidth() << ", " << GetHeight() << "]";
        return ss.str();
    }

    void Bounds::SetPosition(const glm::vec2& position)
    {
        glm::vec2 size = GetSize();
        Min = position;
        Max = position + size;
    }

    float Bounds::GetWidth() const { return Max.x - Min.x; }
    float Bounds::GetHeight() const { return Max.y - Min.y; }
    void Bounds::SetSize(const glm::vec2& size) { Max = Min + size; }
    void Bounds::SetWidth(float width) { Max.x = Min.x + width; }
    void Bounds::SetHeight(float height) { Max.y = Min.y + height; }
    glm::vec2 Bounds::GetCenter() const { return (Min + Max) * 0.5f; }
}
