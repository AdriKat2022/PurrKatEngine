#pragma once

#include "Bounds.h"
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
        // ----------- Conversions -----------
        static glm::vec2 ToVec2(const ImVec2& vec);
        static glm::vec4 ToVec4(const ImVec4& vec);
        
        // ----------- ImGui Draw Functions -----------
        static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float labelWidth = 100.0f);
        static bool DrawBoundsControl(const char* label, Bounds& bounds);
        
        static void ShowApplicationInfoWindow();
        static void ShowDisplayMouseAndWorldPosition(const OrthographicCamera* cam = nullptr);
        static void ShowTransform(const std::string& name, const Transform& transform);
        static void ShowVector3Row(const char* label, const glm::vec3& value);
        static void ShowRendererStatistics(bool showInNewWindow = true, bool showHeader = false);
        static void ShowOrthographicCameraInfos(OrthographicCameraController& cameraController);

        static bool SliderIntControl(const char* label, int& value, int min, int max, int btnStep = 1);
        static bool DragIntControl(const char* label, int& value, int min, int max, int btnStep = 1, float dragSpeed = 1);

        template <typename T, size_t N>
        static bool EnumCombo(const char* label, T& value, const std::array<const char*, N>& items)
        {
            int current = (int)value;
            bool changed = ImGui::Combo(label, &current, items.data(), (int)N);
            if (changed) value = (T)current;
            return changed;
        }

        template <typename T>
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
            else if constexpr (std::is_assignable_v<U, glm::vec2>)
                return DebugControl::Type::Vec2;
            else if constexpr (std::is_assignable_v<U, glm::vec3>)
                return DebugControl::Type::Vec3;
            else if constexpr (std::is_assignable_v<U, glm::vec4>)
                return DebugControl::Type::Vec4;
            else
            {
                static_assert([] { return false; }(), "Unsupported debug control type");
                return DebugControl::Type::None;
            }
        }

#pragma region ImGui Draw Functions
        
        static void AutoImGuiField(const char* label, void* controlPtr, DebugControl::Type type);
        
        template <class T>
        static void AutoImGuiField(const char* label, T* controlPtr)
        {
            AutoImGuiField(label, (void*)controlPtr, GetType<T>());
        }

        template <typename T, typename Getter, typename Setter>
        static void AutoImGuiField(const char* label, T* obj, Getter getter, Setter setter, DebugControl::Type type)
        {
            auto current = (obj->*getter)();
            auto newValue = current;
            AutoImGuiField(label, (void*)&newValue, type);

            if (newValue != current)
                (obj->*setter)(newValue);
        }

        template <typename T, typename Getter, typename Setter>
        static void SliderFloat(const char* label, T* obj, Getter getter, Setter setter, float min, float max)
        {
            auto current = (obj->*getter)();
            auto newValue = current;

            ImGui::SliderFloat(label, &newValue, min, max);

            if (newValue != current)
                (obj->*setter)(newValue);
        }

        template <typename T, typename Getter, typename Setter>
        static void SliderInt(const char* label, T* obj, Getter getter, Setter setter, int min, int max)
        {
            int current = (int)(obj->*getter)();
            int newValue = current;

            ImGui::SliderInt(label, &newValue, min, max);

            if (newValue != current)
                (obj->*setter)(newValue);
        }

#pragma endregion

        template <typename T>
        static void WatchValue(const char* name, T* ptr)
        {
            s_DebugWatchers.push_back({
                .ControlName = name,
                .ControlPtr = (void*)ptr,
                .ControlType = GetType<T>()
            });
        }

        static void ShowWatchedValues(bool useNewWindow = false);

        template <typename T>
        static void AddDebugControl(const char* name, T* ptr)
        {
            s_DebugControls.push_back({
                .ControlName = name,
                .ControlPtr = (void*)ptr,
                .ControlType = GetType<T>()
            });
        }

        static void ShowDebugControls(bool useNewWindow = false);

    private:
        inline static std::vector<DebugControl> s_DebugControls = {};
        inline static std::vector<DebugControl> s_DebugWatchers = {};
    };
}
