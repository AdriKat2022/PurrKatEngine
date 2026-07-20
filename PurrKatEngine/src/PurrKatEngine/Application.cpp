#include "pkepch.h"

#include "Application.h"

#include "Components/Standard2DInputController.h"
#include "Inputs/Time.h"
#include "Logs/InternalLog.h"
#include "Profiling/Profiler.h"
#include "Renderer/Renderer.h"
#include "Window/Window.h"

namespace PurrKatEngine
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        PROFILE_FUNCTION();
        
        PKE_CORE_ASSERT(s_Instance == nullptr, "An application already exists.")
        s_Instance = this;
        
        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallback(PKE_BIND_FUNCTION(OnEvent));

        Renderer::Init();
        
        m_TimeManagerLayer = new TimeManagerLayer();
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        // PKE_CORE_TRACE("EVENT: {}", e.ToString());

        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(PKE_BIND_FUNCTION(OnWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(PKE_BIND_FUNCTION(OnWindowResized));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            // De-increment, as we go backwards, starting at the top of the stack.
            (*--it)->OnEvent(e);
            if (e.IsHandled()) break;
        }
    }

    void Application::Run()
    {
        while (m_IsRunning)
        {
            PROFILE_SCOPE("Application LOOP");
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

            PROFILE_SCOPE("Window Update");
            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
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
        m_IsRunning = false;
        return false;
    }
}
