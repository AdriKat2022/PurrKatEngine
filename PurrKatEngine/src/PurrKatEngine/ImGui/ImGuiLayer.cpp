#include "pkepch.h"
#include "ImGuiLayer.h"

#include "GLFW/glfw3.h"
#include "Platforms/OpenGL/ImgGuiOpenGLRenderer.h"
#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/Logs/Log.h"

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

        io.MouseDrawCursor = true;
        
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
        EventDispatcher dispatcher(e);
        dispatcher.DISPATCH_ON(MouseButtonPressedEvent);
        dispatcher.DISPATCH_ON(MouseButtonReleasedEvent);
        dispatcher.DISPATCH_ON(MouseMovedEvent);
        dispatcher.DISPATCH_ON(MouseScrollEvent);
        dispatcher.DISPATCH_ON(KeyPressedEvent);
        dispatcher.DISPATCH_ON(KeyReleasedEvent);
        dispatcher.DISPATCH_ON(KeyTypedEvent);
        dispatcher.DISPATCH_ON(WindowResizeEvent);
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(e.GetMouseButton(), true);

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(e.GetMouseButton(), false);

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent(e.GetX(), e.GetY());
        
        return false;
    }

    bool ImGuiLayer::OnMouseScrollEvent(MouseScrollEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());
        
        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiKey imGuiKey = e.GetImGuiKey();
        io.AddKeyEvent(imGuiKey, true);

        if (ImGuiKey_LeftCtrl == imGuiKey || ImGuiKey_RightCtrl == imGuiKey)
        {
            io.AddKeyEvent(ImGuiMod_Ctrl, true);
        }
        if (ImGuiKey_LeftShift == imGuiKey || ImGuiKey_RightShift == imGuiKey)
        {
            io.AddKeyEvent(ImGuiMod_Shift, true);
        }
        if (ImGuiKey_LeftAlt == imGuiKey || ImGuiKey_RightAlt == imGuiKey)
        {
            io.AddKeyEvent(ImGuiMod_Alt, true);
        }
        
        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiKey imGuiKey = e.GetImGuiKey();
        io.AddKeyEvent(imGuiKey, false);

        if (ImGuiKey_LeftCtrl == imGuiKey || ImGuiKey_RightCtrl == imGuiKey)
        {
            io.AddKeyEvent(ImGuiMod_Ctrl, false);
        }
        if (ImGuiKey_LeftShift == imGuiKey || ImGuiKey_RightShift == imGuiKey)
        {
            io.AddKeyEvent(ImGuiMod_Shift, false);
        }
        if (ImGuiKey_LeftAlt == imGuiKey || ImGuiKey_RightAlt == imGuiKey)
        {
            io.AddKeyEvent(ImGuiMod_Alt, false);
        }
        
        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharacter(e.GetCharCode());
        
        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)e.GetWidth(), (float)e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        
        return false;
    }
}
