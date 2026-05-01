#pragma once

enum ImGuiKey : int;

namespace PurrKatEngine {
    
    enum class KeyCode : std::uint32_t;
    
    const char* to_string(KeyCode k);
    
    ImGuiKey GlfwCharCodeToImGuiKey(unsigned int charCode);
    KeyCode GlfwCharCodeToKeyCode(unsigned int charCode);
}


