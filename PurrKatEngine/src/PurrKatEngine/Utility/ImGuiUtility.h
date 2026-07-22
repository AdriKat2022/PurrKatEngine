#pragma once
#include "imgui/imgui.h"
#include "PurrKatEngine/Components/Transform.h"

#define DEBUG_CONTROL

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

        //////////// DEBUG CONTROLS //////////////
        
        template<typename T>
        static void AddDebugControl(const char* name, T* ptr)
        {
            DebugControl::Type type;

            if constexpr (std::is_same_v<T, int>)
            {
                type = DebugControl::Type::Int;
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                type = DebugControl::Type::Float;
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                type = DebugControl::Type::Double;
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                type = DebugControl::Type::Bool;
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                type = DebugControl::Type::String;
            }
            else
            {
                static_assert([] { return false; }(), "Unsupported debug control type");
            }

            s_DebugControls.push_back({
                name,
                static_cast<void*>(ptr),
                type
            });
        }
        
        static void ShowDebugControls()
        {
            if (s_DebugControls.empty())
                return;

            if (ImGui::Begin("Debug Controls"))
            {
                for (auto& control : s_DebugControls)
                {
                    switch (control.ControlType)
                    {
                        case DebugControl::Type::Int:
                        {
                            int* value = static_cast<int*>(control.ControlPtr);
                            ImGui::DragInt(control.ControlName.c_str(), value);
                            break;
                        }
                        case DebugControl::Type::Float:
                        case DebugControl::Type::Double:
                        {
                            float* value = static_cast<float*>(control.ControlPtr);
                            ImGui::DragFloat(control.ControlName.c_str(), value, 0.01f);
                            break;
                        }
                        case DebugControl::Type::Bool:
                        {
                            bool* value = static_cast<bool*>(control.ControlPtr);
                            ImGui::Checkbox(control.ControlName.c_str(), value);
                            break;
                        }

                        case DebugControl::Type::String:
                        {
                            std::string* value = static_cast<std::string*>(control.ControlPtr);

                            char buffer[256];
                            std::snprintf(buffer, sizeof(buffer), "%s", value->c_str());

                            if (ImGui::InputText(control.ControlName.c_str(), buffer, sizeof(buffer)))
                                *value = buffer;

                            break;
                        }
                    }
                }
            }

            ImGui::End();

            s_DebugControls.clear();
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
        
    private:
        
        struct DebugControl
        {
            enum class Type
            {
                Int,
                Float,
                Double,
                Bool,
                String
            };

            std::string ControlName;
            void* ControlPtr;
            Type ControlType;
        };
        
        static std::vector<DebugControl> s_DebugControls;
    };
}
