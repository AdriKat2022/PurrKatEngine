#pragma once

#include "imgui.h"
#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/Components/Transform.h"
#include "PurrKatEngine/Renderer/Renderer2D/Renderer2D.h"
#include "PurrKatEngine/Renderer/OrthographicCameraController.h"

#define WATCH_VALUE(value) ::PurrKatEngine::ImGuiUtility::WatchValue(#value, &value)
#define ADD_DEBUG_CONTROL(control) ::PurrKatEngine::ImGuiUtility::AddDebugControl(#control, &control)
#define MAKE_DEBUG_CONTROL(type, control, defaultValue) static type control = defaultValue; ADD_DEBUG_CONTROL(control)

#define AUTO_FIELD_IMGUI(field) ::PurrKatEngine::ImGuiUtility::AutoFieldImGui(#field, &field);

namespace PurrKatEngine
{
    /* Utility class to help with the usage of ImGui. */
    class ImGuiUtility
    {
    public:
        struct DebugControl
        {
            enum class Type
            {
                None,
                Int,
                Float,
                Double,
                Bool,
                String,
                Vec2,
                Vec3,
                Vec4, 
            };

            std::string ControlName;
            void* ControlPtr;
            Type ControlType;
        };
        
    public:
        static void ShowApplicationInfoWindow(const Application& app)
        {
            static bool showStatisticsWindow = true;
            if (ImGui::Begin("Application Infos", &showStatisticsWindow))
            {
                ImGui::Text("Framerate: %.2f", 1/Time::deltaTime);
                ImGui::Text("VSync: %d", app.GetWindow().IsVSync());
            }
            ImGui::End();
        }
        
        static void ShowDisplayMouseAndWorldPosition(const OrthographicCamera* cam = nullptr)
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
        
        static void ShowTransform(const std::string& name, const Transform& transform)
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

                    ShowVector3Row("Position", position);
                    ShowVector3Row("Rotation", rotation);
                    ShowVector3Row("Scale", scale);

                    ImGui::EndTable();
                }
            }
            ImGui::PopID();
        }
        
        static void ShowVector3Row(const char* label, const glm::vec3& value)
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

        static void ShowRendererStatistics(bool showHeader = false)
        {
            bool shouldShow = true;
            
            if (showHeader)
                shouldShow = ImGui::CollapsingHeader("Renderer Statistics", ImGuiTreeNodeFlags_DefaultOpen);
            
            if (!shouldShow)
                return;
            
            auto stats = Renderer2D::GetStatistics();
            ImGui::Text("Draw Calls: %u", stats.DrawCalls);
            ImGui::Text("Quad Count: %u", stats.QuadCount);
            ImGui::Text("Indices: %u", stats.GetIndexCount());
            ImGui::Text("Vertices: %u", stats.GetVertexCount());
            Renderer2D::EndFrameStatistics();
        }
        
        static void ShowOrthographicCameraInfos(OrthographicCameraController& cameraController)
        {
            if (!ImGui::Begin("Orthographic Camera"))
            {
                ImGui::End();
                return;
            }

            // -------------------------------------------------------------------------
            // Transform
            // -------------------------------------------------------------------------
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                glm::vec3 position = cameraController.GetPosition();
                if (ImGui::DragFloat3("Position", (float*)&position, 0.01f))
                    cameraController.SetPosition(position);

                float rotation = cameraController.GetCameraRotation();
                if (ImGui::DragFloat("Rotation", &rotation, 0.5f))
                    cameraController.SetRotation(rotation);

                float zoom = cameraController.GetZoomLevel();
                if (ImGui::DragFloat("Zoom Level", &zoom, 0.01f, 0.01f, 100.0f))
                    cameraController.SetZoomLevel(zoom);
                
                if (ImGui::Button("Reset Position"))
                    cameraController.SetPosition({ 0.0f, 0.0f, 0.0f });

                ImGui::SameLine();

                if (ImGui::Button("Reset Rotation"))
                    cameraController.SetRotation(0.0f);

                ImGui::SameLine();

                if (ImGui::Button("Reset Zoom"))
                    cameraController.SetZoomLevel(1.0f);
            }

            // -------------------------------------------------------------------------
            // Camera
            // -------------------------------------------------------------------------
            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
            {
                const auto& bounds = cameraController.GetCameraBounds();

                ImGui::Text("Bounds");
                ImGui::Indent();
                ImGui::Text("Left   %.3f", bounds.Left);
                ImGui::Text("Right  %.3f", bounds.Right);
                ImGui::Text("Bottom %.3f", bounds.Bottom);
                ImGui::Text("Top    %.3f", bounds.Top);
                ImGui::Unindent();

                ImGui::Spacing();

                ImGui::Text("Size");
                ImGui::SameLine();
                ImGui::TextDisabled("%.3f x %.3f", bounds.GetWidth(), bounds.GetHeight());

                const float aspect = bounds.GetHeight() != 0.0f
                    ? bounds.GetWidth() / bounds.GetHeight()
                    : 0.0f;

                ImGui::Text("Aspect Ratio");
                ImGui::SameLine();
                ImGui::TextDisabled("%.3f", aspect);
            }

            // -------------------------------------------------------------------------
            // Controller
            // -------------------------------------------------------------------------
            if (ImGui::CollapsingHeader("Controller", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Checkbox("Enable Movement", &cameraController.EnableMovement);
                ImGui::Checkbox("Enable Rotation", &cameraController.EnableRotation);
                ImGui::Checkbox("Enable Zoom", &cameraController.EnableZoom);

                ImGui::Spacing();

                const char* modes[] = {
                    "None",
                    "Match Width",
                    "Match Height"
                };

                int mode = (int)cameraController.AspectRatioAdjustment;

                if (ImGui::Combo("Aspect Ratio", &mode, modes, IM_ARRAYSIZE(modes)))
                    cameraController.AspectRatioAdjustment = (AspectRatioAdjustmentMode)mode;
            }

            // -------------------------------------------------------------------------
            // Quick actions
            // -------------------------------------------------------------------------
            // if (ImGui::CollapsingHeader("Actions"))
            // {
            //     
            // }

            ImGui::End();
        }
        
        static bool SliderIntControl(const char* label, int& value, int min, int max, int btnStep = 1)
        {
            bool changed = false;
            ImGui::PushID(label);
            ImGui::Text("%s", label);
            ImGui::SameLine();

            if (ImGui::Button("-"))
            {
                value = std::max(min, value - btnStep);
                changed = true;
            }

            ImGui::SameLine();

            if (ImGui::Button("+"))
            {
                value = std::min(max, value + btnStep);
                changed = true;
            }

            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

            if (ImGui::SliderInt("##value", &value, min, max))
                changed = true;

            ImGui::PopID();
            return changed;
        }
        
        static bool DragIntControl(const char* label, int& value, int min, int max, int btnStep = 1, float dragSpeed = 1)
        {
            bool changed = false;
            ImGui::PushID(label);
            ImGui::Text("%s", label);
            ImGui::SameLine();

            if (ImGui::Button("-"))
            {
                value = std::max(min, value - btnStep);
                changed = true;
            }

            ImGui::SameLine();

            if (ImGui::Button("+"))
            {
                value = std::min(max, value + btnStep);
                changed = true;
            }

            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

            if (ImGui::DragInt("##value", &value, dragSpeed, min, max))
                changed = true;

            ImGui::PopID();
            return changed;
        }
        
        template<typename T, size_t N>
        static bool EnumCombo(const char* label, T& value, const std::array<const char*, N>& items)
        {
            int current = (int)value;
            bool changed = ImGui::Combo(label, &current, items.data(), (int)N);
            if (changed) value = (T)current;
            return changed;
        }

        //////////// DEBUG CONTROLS //////////////
        
        template<typename T>
        static DebugControl::Type GetType()
        {
            using U = std::remove_cvref_t<T>;
            
            if constexpr (std::is_same_v<U, int> || std::is_same_v<U, uint32_t>)
                return DebugControl::Type::Int;
            else if constexpr (std::is_same_v<U, float>)
                return DebugControl::Type::Float;
            else if constexpr (std::is_same_v<U, double>)
                return DebugControl::Type::Double;
            else if constexpr (std::is_same_v<U, bool>)
                return DebugControl::Type::Bool;
            else if constexpr (std::is_same_v<U, std::string>)
                return DebugControl::Type::String;
            else if constexpr (std::is_convertible_v<U, glm::vec2>)
                return DebugControl::Type::Vec2;
            else if constexpr (std::is_convertible_v<U, glm::vec3>)
                return DebugControl::Type::Vec3;
            else if constexpr (std::is_convertible_v<U, glm::vec4>)
                return DebugControl::Type::Vec4;
            else
            {
                static_assert([] { return false; }(), "Unsupported debug control type");
                return DebugControl::Type::None;
            }
        }
        
        #pragma region ImGui Draw Functions
        
        template<class T>
        static void AutoImGuiField(const char* label, T* controlPtr)
        {
            AutoImGuiField(label, (void*)controlPtr, GetType<T>());
        }        
        
        template<typename T, typename Getter, typename Setter>
        static void AutoImGuiField(const char* label, T* obj, Getter getter, Setter setter, DebugControl::Type type)
        {
            auto current = (obj->*getter)();
            auto newValue = current;
            AutoImGuiField(label, (void*)&newValue, type);
            
            if (newValue != current)
                (obj->*setter)(newValue);
        }
        
        static void AutoImGuiField(const char* label, void* controlPtr, DebugControl::Type type)
        {
            switch (type)
            {
                case DebugControl::Type::None:
                    break;
                    
                case DebugControl::Type::Int:
                {
                    int* value = (int*)controlPtr;
                    ImGui::DragInt(label, value);
                    break;
                }
                
                case DebugControl::Type::Float:
                case DebugControl::Type::Double:
                {
                    float* value = (float*)controlPtr;
                    ImGui::DragFloat(label, value, 0.01f);
                    break;
                }
                
                case DebugControl::Type::Bool:
                {
                    bool* value = (bool*)controlPtr;
                    ImGui::Checkbox(label, value);
                    break;
                }
                
                case DebugControl::Type::String:
                {
                    std::string* value = (std::string*)controlPtr;

                    char buffer[256];
                    std::snprintf(buffer, sizeof(buffer), "%s", value->c_str());

                    if (ImGui::InputText(label, buffer, sizeof(buffer)))
                        *value = buffer;

                    break;
                }
                
                case DebugControl::Type::Vec2:
                    ImGui::DragFloat2(label, (float*)controlPtr);
                    break;
                case DebugControl::Type::Vec3:
                    ImGui::DragFloat3(label, (float*)controlPtr);
                    break;
                case DebugControl::Type::Vec4:
                    if (std::string(label).find_last_of("Color") != std::string::npos)
                        ImGui::ColorEdit4(label, (float*)controlPtr);
                    else
                        ImGui::DragFloat4(label, (float*)controlPtr);
                    break;
            }
        }

        template<typename T, typename Getter, typename Setter>
        static void SliderFloat(const char* label, T* obj, Getter getter, Setter setter, float min, float max)
        {
            auto current = (obj->*getter)();
            auto newValue = current;
            
            ImGui::SliderFloat(label, &newValue, min, max);
            
            if (newValue != current)
                (obj->*setter)(newValue);
        }
        
        template<typename T, typename Getter, typename Setter>
        static void SliderInt(const char* label, T* obj, Getter getter, Setter setter, int min, int max)
        {
            int current = (int)(obj->*getter)();
            int newValue = current;
            
            ImGui::SliderInt(label, &newValue, min, max);
            
            if (newValue != current)
                (obj->*setter)(newValue);
        }
        
        #pragma endregion
        
        template<typename T>
        static void WatchValue(const char* name, T* ptr)
        {
            s_DebugWatchers.push_back({
                .ControlName = name,
                .ControlPtr = (void*)ptr,
                .ControlType = GetType<T>()
            });
        }
        
        static void ShowWatchedValues(bool useNewWindow = false)
        {
            if (s_DebugWatchers.empty()) return;
            
            bool opened;
            if (useNewWindow)
            {
                opened = ImGui::Begin("Watched Values");
            }
            else
            {
                opened = ImGui::CollapsingHeader("Watched Values");
            }
            
            if (!opened)
            {
                if (useNewWindow) ImGui::End();
                s_DebugWatchers.clear();
                return;
            }
            
            ImGui::BeginDisabled(true);
            
            for (auto& control : s_DebugWatchers)
            {
                AutoImGuiField(control.ControlName.c_str(), control.ControlPtr, control.ControlType);
            }
            
            ImGui::EndDisabled();
            
            s_DebugWatchers.clear();
        }
        
        template<typename T>
        static void AddDebugControl(const char* name, T* ptr)
        {
            s_DebugControls.push_back({
                .ControlName = name,
                .ControlPtr = (void*)ptr,
                .ControlType = GetType<T>()
            });
        }

        static void ShowDebugControls(bool useNewWindow = false)
        {
            if (s_DebugControls.empty()) return;
            
            bool opened;
            if (useNewWindow)
            {
                opened = ImGui::Begin("Debug Controls");
            }
            else
            {
                opened = ImGui::CollapsingHeader("Debug Controls");
            }
            
            if (!opened)
            {
                if (useNewWindow) ImGui::End();
                s_DebugControls.clear();
                return;
            }
            
            for (auto& control : s_DebugControls)
            {
                AutoImGuiField(control.ControlName.c_str(), control.ControlPtr, control.ControlType);
            }
            
            s_DebugControls.clear();
        }
        
    private:
        inline static std::vector<DebugControl> s_DebugControls = {};
        inline static std::vector<DebugControl> s_DebugWatchers = {};
    };
}
