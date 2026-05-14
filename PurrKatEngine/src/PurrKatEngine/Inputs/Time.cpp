#include "pkepch.h"
#include "Time.h"

#include "GLFW/glfw3.h"

namespace PurrKatEngine
{
    double Time::time = 0;
    // double Time::deltaTime = 0;
    
    TimeManagerLayer::~TimeManagerLayer()
    {
    }

    void TimeManagerLayer::OnAttach()
    {
        Layer::OnAttach();
    }

    void TimeManagerLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    void TimeManagerLayer::OnUpdate()
    {
        Layer::OnUpdate();
        Time::time = glfwGetTime();
        // PKE_CORE_DEBUG("Time: {}, Delta Time: {}", Time::time, Time::deltaTime);
    }

    void TimeManagerLayer::OnImGuiRender()
    {
        Layer::OnImGuiRender();
    }

    void TimeManagerLayer::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }
}
