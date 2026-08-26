#include "pkepch.h"
#include "ImGuiLayer.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "GLFW/glfw3.h"
#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    static bool s_IsImGuiInitialized = false;
    
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        Layer::OnAttach();

        if (s_IsImGuiInitialized) return;
        
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        
        // Setup Dear ImGui style
        SetUpImGuiStyle();
        
        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        s_IsImGuiInitialized = true;
    }

    void ImGuiLayer::OnDetach()
    {
        Layer::OnDetach();
        
        // We may encounter issues if we detach an ImGuiLayer while there are still others.
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    
    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetUpImGuiStyle()
    {
        auto& io = ImGui::GetIO();
        
        // ------------------- Setup Fonts -------------------
        
        io.Fonts->AddFontFromFileTTF("assets/fonts/CascadiaCode/CascadiaCode-Regular.ttf", 18.0f);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/NotoSansJP/NotoSansJP-Regular.ttf", 18.0f);
        
        // Setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
        // style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        // style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        
        
        // ------------------- Setup Colors -------------------
        
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        auto& imguiColors = ImGui::GetStyle().Colors;
        imguiColors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};
        
        imguiColors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        imguiColors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        imguiColors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        
        imguiColors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        imguiColors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        imguiColors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        
        imguiColors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        imguiColors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        imguiColors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0};
        
        imguiColors[ImGuiCol_Tab] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        imguiColors[ImGuiCol_TabHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        imguiColors[ImGuiCol_TabActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        imguiColors[ImGuiCol_TabUnfocused] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        imguiColors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        
        imguiColors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        imguiColors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        imguiColors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    }
}
