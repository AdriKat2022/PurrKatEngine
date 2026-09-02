#pragma once

#include <glm/glm.hpp>
#include "imgui.h"

namespace PurrKatEngine
{
    struct Size
    {
        union
        {
            float x = 0;
            float Width;
        };
        
        union
        {
            float y = 0;
            float Height;
        };

        Size() = default;
        Size(float width, float height): Width(width), Height(height) {}
        
        bool operator==(const Size& o) const { return x == o.x && y == o.y; }
        
        operator glm::vec2() const { return { x, y }; }
        operator ImVec2() const { return { x, y }; }
    };
    
    struct Bounds
    {
        glm::vec2 Min{ 0.0f };
        glm::vec2 Max{ 0.0f };

        Bounds();
        Bounds(const glm::vec2& min, const glm::vec2& max);
        Bounds(float minX, float minY, float maxX, float maxY);
        
        std::string ToString() const;
        
        // Position
        /**
         * Set the position of the bounds by adjusting the Min corner, keeping the size constant.
         * @param position Position of the Min corner of the bounds.
         */
        void SetPosition(const glm::vec2& position);

        // Size
        Size GetSize() const { glm::vec2 vec = Max - Min; return {vec.x, vec.y}; }

        float GetWidth() const;
        float GetHeight() const;

        /**
         * Set the size of the bounds by adjusting the Max corner, keeping the Min corner constant.
         * @param size New size of the bounds. Top Left corner (Min) remains unchanged.
         */
        void SetSize(const glm::vec2& size);
        void SetWidth(float width);
        void SetHeight(float height);

        // Others
        /**
         * Returns the centre point of the bounds.
         * @return The centre of the bounds.
         */
        glm::vec2 GetCenter() const;
    };
}
