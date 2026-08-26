#pragma once

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
                
            return std::distance(std::begin(container), it);
        }
        
    };
}
