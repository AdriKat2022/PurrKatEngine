#include "pkepch.h"
#include "WindowsWindow.h"

namespace PurrKatEngine
{
    CREATE_WINDOW_SETUP(WindowsWindow)

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::OnUpdate()
    {
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
    }

    void WindowsWindow::Shutdown()
    {
    }
}
