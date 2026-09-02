#pragma once

#define PKE_CONVERT_VEC2(vec) {vec.x, vec.y}
#define PKE_CONVERT_VEC3(vec) {vec.x, vec.y, vec.z}
#define PKE_CONVERT_VEC4(vec) {vec.x, vec.y, vec.z, vec.w}

namespace PurrKatEngine
{
    class ArrayUtility
    {
    public:
        template<typename Container, typename T>
        static int IndexOf(const T& itemToFind, const Container& container)
        {
            auto it = std::find_if(
                std::begin(container),
                std::end(container),
                [&itemToFind](const auto& item) { return item == itemToFind; }
            );

            if (it == std::end(container))
                return -1; // Not found
                
            return (int)std::distance(std::begin(container), it);
        }
        
    };
}
