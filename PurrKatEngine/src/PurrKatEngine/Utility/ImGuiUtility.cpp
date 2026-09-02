#include "pkepch.h"

#include "ImGuiUtility.h"
#include "imgui_internal.h"

namespace PurrKatEngine
{
    glm::vec2 ImGuiUtility::ToVec2(const ImVec2& vec)
    {
        return {vec.x, vec.y};
    }

    glm::vec4 ImGuiUtility::ToVec4(const ImVec4& vec)
    {
        return {vec.x, vec.y, vec.z, vec.w};
    }

    bool ImGuiUtility::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float labelWidth)
    {
        bool changed = false;
        
        ImGui::PushID(label.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelWidth);
        
        ImGui::Text(label.c_str());
        
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
        
        float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};
        
        // X COMPONENT
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.6f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.7f, 0.2f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.3f, 0.2f, 1.0f});
        if (ImGui::Button("X", buttonSize))
        {
            values.x = resetValue;
            changed |= true;
        }
        ImGui::SameLine();
        changed |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);
        
        ImGui::SameLine();
        
        // Y COMPONENT
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.15f, 0.6f, 0.1f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.15f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.15f, 0.8f, 0.3f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
        {
            values.y = resetValue;
            changed |= true;
        }
        ImGui::SameLine();
        changed |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);
        
        ImGui::SameLine();
        
        // Z COMPONENT
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.15f, 0.6f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.15f, 0.7f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.3f, 0.15f, 0.8f, 1.0f});
        if (ImGui::Button("Z", buttonSize))
        {
            values.z = resetValue;
            changed |= true;
        }
        ImGui::SameLine();
        changed |= ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);
        
        ImGui::PopStyleVar();
        
        ImGui::Columns(1);
        
        ImGui::PopID();
        
        return changed;
    }

    bool ImGuiUtility::DrawBoundsControl(const char* label, Bounds& bounds)
    {
        bool changed = false;
        if (!ImGui::BeginTable(label, 4, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersOuter))
            return false;
        
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 20);
        ImGui::TableSetupColumn("Min");
        ImGui::TableSetupColumn("Max");
        ImGui::TableSetupColumn("Size");
        
        // X
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted("X");
        ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##MinX", &bounds.Min.x, 0.1f);
        ImGui::TableSetColumnIndex(2); changed |= ImGui::DragFloat("##MaxX", &bounds.Max.x, 0.1f);
        ImGui::TableSetColumnIndex(3); ImGui::Text("%.3f", bounds.GetWidth());
        ImGui::TableNextRow();
        
        // Y
        ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted("Y"); 
        ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##MinY", &bounds.Min.y, 0.1f);
        ImGui::TableSetColumnIndex(2); changed |= ImGui::DragFloat("##MaxY", &bounds.Max.y, 0.1f);
        ImGui::TableSetColumnIndex(3); ImGui::Text("%.3f", bounds.GetHeight());
        ImGui::EndTable();
        return changed;
    }

    void ImGuiUtility::ShowApplicationInfoWindow()
    {
        static bool showStatisticsWindow = true;
        if (ImGui::Begin("Application Infos", &showStatisticsWindow))
        {
            ImGui::Text("Framerate: %.2f", 1 / Time::deltaTime);
            ImGui::Text("VSync: %d", Application::Get().GetWindow().IsVSync());
        }
        ImGui::End();
    }

    void ImGuiUtility::ShowDisplayMouseAndWorldPosition(const OrthographicCamera* cam)
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

    void ImGuiUtility::ShowTransform(const std::string& name, const Transform& transform)
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

    void ImGuiUtility::ShowVector3Row(const char* label, const glm::vec3& value)
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

    void ImGuiUtility::ShowRendererStatistics(bool showInNewWindow, bool showHeader)
    {
        bool shouldShow = true;

        if (showInNewWindow)
            shouldShow = ImGui::Begin("Renderer Statistics", &shouldShow, ImGuiWindowFlags_AlwaysAutoResize);
        
        if (showHeader)
            shouldShow = ImGui::CollapsingHeader("Renderer Statistics", ImGuiTreeNodeFlags_DefaultOpen);

        if (shouldShow)
        {
            auto stats = Renderer2D::GetStatistics();
            ImGui::Text("Draw Calls: %u", stats.DrawCalls);
            ImGui::Text("Quad Count: %u", stats.QuadCount);
            ImGui::Text("Indices: %u", stats.GetIndexCount());
            ImGui::Text("Vertices: %u", stats.GetVertexCount());
            Renderer2D::EndFrameStatistics();
        }
        
        
        if (showInNewWindow)
            ImGui::End();
    }

    void ImGuiUtility::ShowOrthographicCameraInfos(OrthographicCameraController& cameraController)
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
                cameraController.SetPosition({0.0f, 0.0f, 0.0f});

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

    bool ImGuiUtility::SliderIntControl(const char* label, int& value, int min, int max, int btnStep)
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

    bool ImGuiUtility::DragIntControl(const char* label, int& value, int min, int max, int btnStep, float dragSpeed)
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

    void ImGuiUtility::AutoImGuiField(const char* label, void* controlPtr, DebugControl::Type type)
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

    void ImGuiUtility::ShowWatchedValues(bool useNewWindow)
    {
        if (s_DebugWatchers.empty())
            return;

        bool opened;
        if (useNewWindow)
            opened = ImGui::Begin("Watched Values");
        else
            opened = ImGui::CollapsingHeader("Watched Values");

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

    void ImGuiUtility::ShowDebugControls(bool useNewWindow)
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
}
