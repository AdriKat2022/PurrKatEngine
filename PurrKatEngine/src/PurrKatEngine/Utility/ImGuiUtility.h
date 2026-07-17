#pragma once
#include "imgui/imgui.h"
#include "PurrKatEngine/Components/Transform.h"

namespace PurrKatEngine
{
    /* Utility class to help with the usage of ImGui. */
    class ImGuiUtility
    {
    public:
        static void DisplayTransform(const std::string& name, const Transform& transform)
        {
            const glm::vec3& position = transform.GetPosition();
            const glm::vec3& rotation = transform.GetRotation();
            const glm::vec3& scale = transform.GetScale();

            ImGui::PushID(&transform);
            if (ImGui::CollapsingHeader(name.c_str()))
            {
                if (ImGui::BeginTable("TransformProperties", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                {
                    ImGui::TableSetupColumn("Transform");
                    ImGui::TableSetupColumn("X");
                    ImGui::TableSetupColumn("Y");
                    ImGui::TableSetupColumn("Z");
                    ImGui::TableHeadersRow();

                    DisplayVector3Row("Position", position);
                    DisplayVector3Row("Rotation", rotation);
                    DisplayVector3Row("Scale", scale);

                    ImGui::EndTable();
                }
            }
            ImGui::PopID();
        }

        template<typename T, size_t N>
        static bool EnumCombo(const char* label, T& value, const std::array<const char*, N>& items)
        {
            int current = static_cast<int>(value);
            bool changed = ImGui::Combo(label, &current, items.data(), (int)N);
            if (changed) value = static_cast<T>(current);
            return changed;
        }

    private:
        static void DisplayVector3Row(const char* label, const glm::vec3& value)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.3f", value.x);
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.3f", value.y);
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.3f", value.z);
        }
    };
}
