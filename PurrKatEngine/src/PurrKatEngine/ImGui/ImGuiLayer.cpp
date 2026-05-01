#include "pkepch.h"
#include "ImGuiLayer.h"

#include "GLFW/glfw3.h"
#include "Platforms/OpenGL/ImgGuiOpenGLRenderer.h"
#include "PurrKatEngine/Application.h"

namespace PurrKatEngine
{
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        Layer::OnAttach();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.BackendPlatformName = "imgui_impl_glfw";

        ImGui_ImplOpenGL3_Init();
    }
    
    void ImGuiLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    void ImGuiLayer::OnUpdate()
    {
        Layer::OnUpdate();

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0 ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        static bool test = true;
        ImGui::ShowDemoWindow(&test);
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        Layer::OnEvent(e);
    }
}
