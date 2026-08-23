#include "pkepch.h"

#include "Application.h"

#include "ImGui/ImGuiLayer.h"
#include "Inputs/Time.h"
#include "Logs/InternalLog.h"
#include "Profiling/Profiler.h"
#include "Renderer/Renderer.h"
#include "Window/Window.h"

namespace PurrKatEngine
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& appName)
    {
        PROFILE_FUNCTION();
        
        PKE_CORE_ASSERT(s_Instance == nullptr, "An application already exists.")
        s_Instance = this;
        
        m_Window = CreateScope(Window::Create({ .Title = appName }));
        m_Window->SetEventCallback(PKE_BIND_FUNCTION(OnEvent));

        Renderer::Init();
        
        // TimeManagerLayer's got a special treatment
        // We handle it manually in the update loop instead of handing it over the layer stack system.
        m_TimeManagerLayer = new TimeManagerLayer();
        
        // Same for the imgui layer, but we need the onAttach/onDetach methods
        // We could call them also manually, but it's cleaner to leave that to the layer stack system to handle.
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        PROFILE_FUNCTION();
        
        // PKE_CORE_TRACE("EVENT: {}", e.ToString());

        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(PKE_BIND_FUNCTION(OnWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(PKE_BIND_FUNCTION(OnWindowResized));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            // De-increment, as we go backwards, starting at the top of the stack.
            (*--it)->OnEvent(e);
            if (e.Handled) break;
        }
    }

    void Application::Run()
    {
        while (m_IsRunning)
        {
            PROFILE_SCOPE("Application Loop");
            m_TimeManagerLayer->OnUpdate();
            
            if (!m_IsMinimized)
            {
                PROFILE_SCOPE("Layer Update");
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnUpdate();
                }
            }
            
            {
                PROFILE_SCOPE("ImGui Update");
            
                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }

            {
                PROFILE_SCOPE("Window Update");
                m_Window->OnUpdate();
            }
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        PROFILE_FUNCTION();
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        PROFILE_FUNCTION();
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::QuitApplication()
    {
        m_IsRunning = false;
    }

    bool Application::OnWindowResized(WindowResizeEvent& windowResizeEvent)
    {
        PROFILE_FUNCTION();
        if (windowResizeEvent.GetWidth() == 0 || windowResizeEvent.GetHeight() == 0)
        {
            PKE_CORE_DEBUG("Window is minimised!");
            m_IsMinimized = true;
            return false;
        }
        
        m_IsMinimized = false;
        Renderer::OnWindowResize(windowResizeEvent.GetWidth(), windowResizeEvent.GetHeight());
        
        return false;
    }
    
    bool Application::OnWindowClosed(WindowCloseEvent& windowCloseEvent)
    {
        QuitApplication();
        return false;
    }
}
