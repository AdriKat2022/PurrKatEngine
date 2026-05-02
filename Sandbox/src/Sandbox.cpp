#include <PurrKatEngine.h>
#include "imgui/imgui.h"

class ExampleSandboxLayer : public PurrKatEngine::ImGuiLayer
{
public:
    ExampleSandboxLayer() = default;

    void OnImGuiRender() override
    {
        static bool showWindow = true;
        ImGui::Begin("Hello, I'm a window!", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImGui::Text("This is some text in the window.");
            static bool showBelowTest = false;
            ImGui::Checkbox("Demo Window", &showBelowTest);

            if (showBelowTest)
            {
                static ImColor testColor = ImColor(0.4f, 0.7f, 0.0f, 1.0f);
                ImGui::ColorButton("Test Color Button", ImVec4(0.4f, 0.7f, 0.0f, 1.0f));
                ImGui::ColorPicker3("Test Color Picker", (float*)&testColor);
                ImGui::TextColored(testColor, "WOW!");
                if (ImGui::ArrowButton("Test Arrow Button", ImGuiDir_Right))
                {
                    showWindow = !showWindow;
                    PKE_LOG_INFO("Arrow button clicked! {}", showWindow);
                }
            }
        }
        ImGui::End();
    }

    void OnUpdate() override
    {
        // PurrKatEngine::Log::LogDebug("ExampleSandboxLayer::OnUpdate");
        bool isTabPressed = PurrKatEngine::Input::IsKeyPressed(PKE_KEY_Keypad0);
        if (isTabPressed)
        {
            PKE_LOG_DEBUG("Keypad 0 is pressed!");
            glm::vec2 mousePos = PurrKatEngine::Input::GetMousePosition();
            PKE_LOG_INFO("Mouse Position: ({0}, {1})", mousePos.x, mousePos.y);
        }
    }

    void OnEvent(PurrKatEngine::Event& e) override
    {
        // PKE_LOG_DEBUG("ExampleSandboxLayer::OnEvent {}", e.ToString());
    }
};

class Sandbox : public PurrKatEngine::Application
{
public:
    Sandbox()
    {
        PurrKatEngine::Log::LogTrace("Sandbox application start. Hey, that's me! A log from the client!");
        PushLayer(new ExampleSandboxLayer());
    }
};

CREATE_APPLICATION_FROM_CLASS(Sandbox);
