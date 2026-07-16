#pragma once
#include "imgui/imgui.h"

namespace PurrKatEngine
{
    /* Utility class to help with the usage of ImGui. */
    class ImGuiUtility
    {
    public:
        template<typename T, size_t N>
        static bool EnumCombo(const char* label, T& value, const std::array<const char*, N>& items)
        {
            int current = static_cast<int>(value);
            bool changed = ImGui::Combo(label, &current, items.data(), (int)N);
            if (changed) value = static_cast<T>(current);
            return changed;
        }
    };
}
