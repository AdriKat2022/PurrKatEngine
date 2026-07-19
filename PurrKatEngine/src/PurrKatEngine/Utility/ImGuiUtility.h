#pragma once
#include "imgui/imgui.h"
#include "PurrKatEngine/Components/Transform.h"

namespace PurrKatEngine
{
    /* Utility class to help with the usage of ImGui. */
    class ImGuiUtility
    {
    public:
        static void ApplicationInfoWindow(const Application& app)
        {
            static bool showStatisticsWindow = true;
            if (ImGui::Begin("Application Infos", &showStatisticsWindow))
            {
                ImGui::Text("Framerate: %.2f", 1/Time::deltaTime);
                ImGui::Text("VSync: %d", app.GetWindow().IsVSync());
            }
            ImGui::End();
        }
        
        static void DisplayMouseAndWorldPosition(const OrthographicCamera* cam = nullptr)
        {
            ImVec2 mousePos = ImGui::GetMousePos();
            glm::vec3 worldPos = cam ? cam->ScreenToWorldPosition({mousePos.x, mousePos.y}) : glm::vec3(0);
            
            if (ImGui::BeginTable("MousePosition", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableHeadersRow();
                
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted("Mouse Position");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.3f", mousePos.x);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.3f", mousePos.y);
                
                if (cam)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("World Position");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%.3f", worldPos.x);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%.3f", worldPos.y);
                }

                ImGui::EndTable();
            }
            
        }
        
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
