#include "pkepch.h"
#include "Keys.h"

#include "GLFW/glfw3.h"
#include "Platforms/OpenGL/ImgGuiOpenGLRenderer.h"

namespace PurrKatEngine
{
    const char* to_string(KeyCode k)
    {
        // Simple implementation for now: This function is primarily for debugging.
        switch (k)
        {
            case KeyCode::None: return "None";
            case KeyCode::Tab: return "Tab";
            case KeyCode::Space: return "Space";
            case KeyCode::Enter: return "Enter";
            case KeyCode::Escape: return "Escape";
            default: return "Unknown";
        }
    }
    
    int GlfwCharCodeToImGuiKey(unsigned int charCode)
    {
        switch (charCode)
        {
            case GLFW_KEY_TAB: return ImGuiKey_Tab;
            case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
            case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
            case GLFW_KEY_UP: return ImGuiKey_UpArrow;
            case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
            case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
            case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
            case GLFW_KEY_HOME: return ImGuiKey_Home;
            case GLFW_KEY_END: return ImGuiKey_End;
            case GLFW_KEY_INSERT: return ImGuiKey_Insert;
            case GLFW_KEY_DELETE: return ImGuiKey_Delete;
            case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
            case GLFW_KEY_SPACE: return ImGuiKey_Space;
            case GLFW_KEY_ENTER: return ImGuiKey_Enter;
            case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
            case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
            case GLFW_KEY_COMMA: return ImGuiKey_Comma;
            case GLFW_KEY_MINUS: return ImGuiKey_Minus;
            case GLFW_KEY_PERIOD: return ImGuiKey_Period;
            case GLFW_KEY_SLASH: return ImGuiKey_Slash;
            case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
            case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
            case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
            case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
            case GLFW_KEY_WORLD_1: return ImGuiKey_Oem102;
            case GLFW_KEY_WORLD_2: return ImGuiKey_Oem102;
            case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
            case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
            case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
            case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
            case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
            case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
            case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
            case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
            case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
            case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
            case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
            case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
            case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
            case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
            case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
            case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
            case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
            case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
            case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
            case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
            case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
            case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
            case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
            case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
            case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
            case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
            case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
            case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
            case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
            case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
            case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
            case GLFW_KEY_MENU: return ImGuiKey_Menu;
            case GLFW_KEY_0: return ImGuiKey_0;
            case GLFW_KEY_1: return ImGuiKey_1;
            case GLFW_KEY_2: return ImGuiKey_2;
            case GLFW_KEY_3: return ImGuiKey_3;
            case GLFW_KEY_4: return ImGuiKey_4;
            case GLFW_KEY_5: return ImGuiKey_5;
            case GLFW_KEY_6: return ImGuiKey_6;
            case GLFW_KEY_7: return ImGuiKey_7;
            case GLFW_KEY_8: return ImGuiKey_8;
            case GLFW_KEY_9: return ImGuiKey_9;
            case GLFW_KEY_A: return ImGuiKey_A;
            case GLFW_KEY_B: return ImGuiKey_B;
            case GLFW_KEY_C: return ImGuiKey_C;
            case GLFW_KEY_D: return ImGuiKey_D;
            case GLFW_KEY_E: return ImGuiKey_E;
            case GLFW_KEY_F: return ImGuiKey_F;
            case GLFW_KEY_G: return ImGuiKey_G;
            case GLFW_KEY_H: return ImGuiKey_H;
            case GLFW_KEY_I: return ImGuiKey_I;
            case GLFW_KEY_J: return ImGuiKey_J;
            case GLFW_KEY_K: return ImGuiKey_K;
            case GLFW_KEY_L: return ImGuiKey_L;
            case GLFW_KEY_M: return ImGuiKey_M;
            case GLFW_KEY_N: return ImGuiKey_N;
            case GLFW_KEY_O: return ImGuiKey_O;
            case GLFW_KEY_P: return ImGuiKey_P;
            case GLFW_KEY_Q: return ImGuiKey_Q;
            case GLFW_KEY_R: return ImGuiKey_R;
            case GLFW_KEY_S: return ImGuiKey_S;
            case GLFW_KEY_T: return ImGuiKey_T;
            case GLFW_KEY_U: return ImGuiKey_U;
            case GLFW_KEY_V: return ImGuiKey_V;
            case GLFW_KEY_W: return ImGuiKey_W;
            case GLFW_KEY_X: return ImGuiKey_X;
            case GLFW_KEY_Y: return ImGuiKey_Y;
            case GLFW_KEY_Z: return ImGuiKey_Z;
            case GLFW_KEY_F1: return ImGuiKey_F1;
            case GLFW_KEY_F2: return ImGuiKey_F2;
            case GLFW_KEY_F3: return ImGuiKey_F3;
            case GLFW_KEY_F4: return ImGuiKey_F4;
            case GLFW_KEY_F5: return ImGuiKey_F5;
            case GLFW_KEY_F6: return ImGuiKey_F6;
            case GLFW_KEY_F7: return ImGuiKey_F7;
            case GLFW_KEY_F8: return ImGuiKey_F8;
            case GLFW_KEY_F9: return ImGuiKey_F9;
            case GLFW_KEY_F10: return ImGuiKey_F10;
            case GLFW_KEY_F11: return ImGuiKey_F11;
            case GLFW_KEY_F12: return ImGuiKey_F12;
            case GLFW_KEY_F13: return ImGuiKey_F13;
            case GLFW_KEY_F14: return ImGuiKey_F14;
            case GLFW_KEY_F15: return ImGuiKey_F15;
            case GLFW_KEY_F16: return ImGuiKey_F16;
            case GLFW_KEY_F17: return ImGuiKey_F17;
            case GLFW_KEY_F18: return ImGuiKey_F18;
            case GLFW_KEY_F19: return ImGuiKey_F19;
            case GLFW_KEY_F20: return ImGuiKey_F20;
            case GLFW_KEY_F21: return ImGuiKey_F21;
            case GLFW_KEY_F22: return ImGuiKey_F22;
            case GLFW_KEY_F23: return ImGuiKey_F23;
            case GLFW_KEY_F24: return ImGuiKey_F24;
            default: return static_cast<int>(ImGuiKey_None);
        }
    }
    
    KeyCode GlfwCharCodeToKeyCode(unsigned int charCode)
    {
        switch (charCode)
        {
            // Special keys
            case GLFW_KEY_TAB:              return KeyCode::Tab;
            case GLFW_KEY_LEFT:             return KeyCode::LeftArrow;
            case GLFW_KEY_RIGHT:            return KeyCode::RightArrow;
            case GLFW_KEY_UP:               return KeyCode::UpArrow;
            case GLFW_KEY_DOWN:             return KeyCode::DownArrow;
            case GLFW_KEY_PAGE_UP:          return KeyCode::PageUp;
            case GLFW_KEY_PAGE_DOWN:        return KeyCode::PageDown;
            case GLFW_KEY_HOME:             return KeyCode::Home;
            case GLFW_KEY_END:              return KeyCode::End;
            case GLFW_KEY_INSERT:           return KeyCode::Insert;
            case GLFW_KEY_DELETE:           return KeyCode::Delete;
            case GLFW_KEY_BACKSPACE:        return KeyCode::Backspace;
            case GLFW_KEY_SPACE:            return KeyCode::Space;
            case GLFW_KEY_ENTER:            return KeyCode::Enter;
            case GLFW_KEY_ESCAPE:           return KeyCode::Escape;

            // Modifiers
            case GLFW_KEY_LEFT_CONTROL:     return KeyCode::LeftCtrl;
            case GLFW_KEY_LEFT_SHIFT:       return KeyCode::LeftShift;
            case GLFW_KEY_LEFT_ALT:         return KeyCode::LeftAlt;
            case GLFW_KEY_LEFT_SUPER:       return KeyCode::LeftSuper;
            case GLFW_KEY_RIGHT_CONTROL:    return KeyCode::RightCtrl;
            case GLFW_KEY_RIGHT_SHIFT:      return KeyCode::RightShift;
            case GLFW_KEY_RIGHT_ALT:        return KeyCode::RightAlt;
            case GLFW_KEY_RIGHT_SUPER:      return KeyCode::RightSuper;
            case GLFW_KEY_MENU:             return KeyCode::Menu;

            // Numbers (0-9)
            case GLFW_KEY_0:                return KeyCode::Key0;
            case GLFW_KEY_1:                return KeyCode::Key1;
            case GLFW_KEY_2:                return KeyCode::Key2;
            case GLFW_KEY_3:                return KeyCode::Key3;
            case GLFW_KEY_4:                return KeyCode::Key4;
            case GLFW_KEY_5:                return KeyCode::Key5;
            case GLFW_KEY_6:                return KeyCode::Key6;
            case GLFW_KEY_7:                return KeyCode::Key7;
            case GLFW_KEY_8:                return KeyCode::Key8;
            case GLFW_KEY_9:                return KeyCode::Key9;

            // Letters (A-Z)
            case GLFW_KEY_A:                return KeyCode::A;
            case GLFW_KEY_B:                return KeyCode::B;
            case GLFW_KEY_C:                return KeyCode::C;
            case GLFW_KEY_D:                return KeyCode::D;
            case GLFW_KEY_E:                return KeyCode::E;
            case GLFW_KEY_F:                return KeyCode::F;
            case GLFW_KEY_G:                return KeyCode::G;
            case GLFW_KEY_H:                return KeyCode::H;
            case GLFW_KEY_I:                return KeyCode::I;
            case GLFW_KEY_J:                return KeyCode::J;
            case GLFW_KEY_K:                return KeyCode::K;
            case GLFW_KEY_L:                return KeyCode::L;
            case GLFW_KEY_M:                return KeyCode::M;
            case GLFW_KEY_N:                return KeyCode::N;
            case GLFW_KEY_O:                return KeyCode::O;
            case GLFW_KEY_P:                return KeyCode::P;
            case GLFW_KEY_Q:                return KeyCode::Q;
            case GLFW_KEY_R:                return KeyCode::R;
            case GLFW_KEY_S:                return KeyCode::S;
            case GLFW_KEY_T:                return KeyCode::T;
            case GLFW_KEY_U:                return KeyCode::U;
            case GLFW_KEY_V:                return KeyCode::V;
            case GLFW_KEY_W:                return KeyCode::W;
            case GLFW_KEY_X:                return KeyCode::X;
            case GLFW_KEY_Y:                return KeyCode::Y;
            case GLFW_KEY_Z:                return KeyCode::Z;

            // Function keys (F1-F24)
            case GLFW_KEY_F1:               return KeyCode::F1;
            case GLFW_KEY_F2:               return KeyCode::F2;
            case GLFW_KEY_F3:               return KeyCode::F3;
            case GLFW_KEY_F4:               return KeyCode::F4;
            case GLFW_KEY_F5:               return KeyCode::F5;
            case GLFW_KEY_F6:               return KeyCode::F6;
            case GLFW_KEY_F7:               return KeyCode::F7;
            case GLFW_KEY_F8:               return KeyCode::F8;
            case GLFW_KEY_F9:               return KeyCode::F9;
            case GLFW_KEY_F10:              return KeyCode::F10;
            case GLFW_KEY_F11:              return KeyCode::F11;
            case GLFW_KEY_F12:              return KeyCode::F12;
            case GLFW_KEY_F13:              return KeyCode::F13;
            case GLFW_KEY_F14:              return KeyCode::F14;
            case GLFW_KEY_F15:              return KeyCode::F15;
            case GLFW_KEY_F16:              return KeyCode::F16;
            case GLFW_KEY_F17:              return KeyCode::F17;
            case GLFW_KEY_F18:              return KeyCode::F18;
            case GLFW_KEY_F19:              return KeyCode::F19;
            case GLFW_KEY_F20:              return KeyCode::F20;
            case GLFW_KEY_F21:              return KeyCode::F21;
            case GLFW_KEY_F22:              return KeyCode::F22;
            case GLFW_KEY_F23:              return KeyCode::F23;
            case GLFW_KEY_F24:              return KeyCode::F24;

            // Punctuation
            case GLFW_KEY_APOSTROPHE:       return KeyCode::Apostrophe;
            case GLFW_KEY_COMMA:            return KeyCode::Comma;
            case GLFW_KEY_MINUS:            return KeyCode::Minus;
            case GLFW_KEY_PERIOD:           return KeyCode::Period;
            case GLFW_KEY_SLASH:            return KeyCode::Slash;
            case GLFW_KEY_SEMICOLON:        return KeyCode::Semicolon;
            case GLFW_KEY_EQUAL:            return KeyCode::Equal;
            case GLFW_KEY_LEFT_BRACKET:     return KeyCode::LeftBracket;
            case GLFW_KEY_BACKSLASH:        return KeyCode::Backslash;
            case GLFW_KEY_RIGHT_BRACKET:    return KeyCode::RightBracket;
            case GLFW_KEY_GRAVE_ACCENT:     return KeyCode::GraveAccent;

            // Lock keys
            case GLFW_KEY_CAPS_LOCK:        return KeyCode::CapsLock;
            case GLFW_KEY_SCROLL_LOCK:      return KeyCode::ScrollLock;
            case GLFW_KEY_NUM_LOCK:         return KeyCode::NumLock;
            case GLFW_KEY_PRINT_SCREEN:     return KeyCode::PrintScreen;
            case GLFW_KEY_PAUSE:            return KeyCode::Pause;

            // Keypad (0-9)
            case GLFW_KEY_KP_0:             return KeyCode::Keypad0;
            case GLFW_KEY_KP_1:             return KeyCode::Keypad1;
            case GLFW_KEY_KP_2:             return KeyCode::Keypad2;
            case GLFW_KEY_KP_3:             return KeyCode::Keypad3;
            case GLFW_KEY_KP_4:             return KeyCode::Keypad4;
            case GLFW_KEY_KP_5:             return KeyCode::Keypad5;
            case GLFW_KEY_KP_6:             return KeyCode::Keypad6;
            case GLFW_KEY_KP_7:             return KeyCode::Keypad7;
            case GLFW_KEY_KP_8:             return KeyCode::Keypad8;
            case GLFW_KEY_KP_9:             return KeyCode::Keypad9;

            // Keypad operations
            case GLFW_KEY_KP_DECIMAL:       return KeyCode::KeypadDecimal;
            case GLFW_KEY_KP_DIVIDE:        return KeyCode::KeypadDivide;
            case GLFW_KEY_KP_MULTIPLY:      return KeyCode::KeypadMultiply;
            case GLFW_KEY_KP_SUBTRACT:      return KeyCode::KeypadSubtract;
            case GLFW_KEY_KP_ADD:           return KeyCode::KeypadAdd;
            case GLFW_KEY_KP_ENTER:         return KeyCode::KeypadEnter;
            case GLFW_KEY_KP_EQUAL:         return KeyCode::KeypadEqual;

            // Default case: return None
            default:                        return KeyCode::None;
        }
    }

}
