#include "pkepch.h"
#include "Keys.h"

#include "GLFW/glfw3.h"
#include "Platforms/OpenGL/ImgGuiOpenGLRenderer.h"

namespace PurrKatEngine
{
    const char* to_string(KeyCode keyCode)
    {
        switch (keyCode)
        {
            case KeyCode::None: return "None";
            case KeyCode::Tab: return "Tab";
            case KeyCode::Space: return "Space";
            case KeyCode::Enter: return "Enter";
            case KeyCode::Escape: return "Escape";
            case KeyCode::LeftArrow: return "LeftArrow";
            case KeyCode::RightArrow: return "RightArrow";
            case KeyCode::UpArrow: return "UpArrow";
            case KeyCode::DownArrow: return "DownArrow";
            case KeyCode::PageUp: return "PageUp";
            case KeyCode::PageDown: return "PageDown";
            case KeyCode::Home: return "Home";
            case KeyCode::End: return "End";
            case KeyCode::Insert: return "Insert";
            case KeyCode::Delete: return "Delete";
            case KeyCode::Backspace: return "Backspace";
            case KeyCode::LeftCtrl: return "LeftCtrl";
            case KeyCode::LeftShift: return "LeftShift";
            case KeyCode::LeftAlt: return "LeftAlt";
            case KeyCode::LeftSuper: return "LeftSuper";
            case KeyCode::RightCtrl: return "RightCtrl";
            case KeyCode::RightShift: return "RightShift";
            case KeyCode::RightAlt: return "RightAlt";
            case KeyCode::RightSuper: return "RightSuper";
            case KeyCode::Menu: return "Menu";
            case KeyCode::Key0: return "Key0";
            case KeyCode::Key1: return "Key1";
            case KeyCode::Key2: return "Key2";
            case KeyCode::Key3: return "Key3";
            case KeyCode::Key4: return "Key4";
            case KeyCode::Key5: return "Key5";
            case KeyCode::Key6: return "Key6";
            case KeyCode::Key7: return "Key7";
            case KeyCode::Key8: return "Key8";
            case KeyCode::Key9: return "Key9";
            case KeyCode::A: return "A";
            case KeyCode::B: return "B";
             case KeyCode::C: return "C";
             case KeyCode::D: return "D";
            case KeyCode::E: return "E";
            case KeyCode::F: return "F";
            case KeyCode::G: return "G";
            case KeyCode::H: return "H";
            case KeyCode::I: return "I";
            case KeyCode::J: return "J";
            case KeyCode::K: return "K";
            case KeyCode::L: return "L";
            case KeyCode::M: return "M";
            case KeyCode::N: return "N";
            case KeyCode::O: return "O";
            case KeyCode::P: return "P";
            case KeyCode::Q: return "Q";
            case KeyCode::R: return "R";
            case KeyCode::S: return "S";
            case KeyCode::T: return "T";
            case KeyCode::U: return "U";
            case KeyCode::V: return "V";
            case KeyCode::W: return "W";
            case KeyCode::X: return "X";
            case KeyCode::Y: return "Y";
            case KeyCode::Z: return "Z";
            case KeyCode::F1: return "F1";
            case KeyCode::F2: return "F2";
            case KeyCode::F3: return "F3";
            case KeyCode::F4: return "F4";
            case KeyCode::F5: return "F5";
            case KeyCode::F6: return "F6";
            case KeyCode::F7: return "F7";
            case KeyCode::F8: return "F8";
            case KeyCode::F9: return "F9";
            case KeyCode::F10: return "F10";
            case KeyCode::F11: return "F11";
            case KeyCode::F12: return "F12";
            case KeyCode::F13: return "F13";
            case KeyCode::F14: return "F14";
            case KeyCode::F15: return "F15";
            case KeyCode::F16: return "F16";
            case KeyCode::F17: return "F17";
            case KeyCode::F18: return "F18";
            case KeyCode::F19: return "F19";
            case KeyCode::F20: return "F20";
            case KeyCode::F21: return "F21";
            case KeyCode::F22: return "F22";
            case KeyCode::F23: return "F23";
            case KeyCode::F24: return "F24";
            case KeyCode::Apostrophe: return "Apostrophe";
            case KeyCode::Comma: return "Comma";
            case KeyCode::Minus: return "Minus";
            case KeyCode::Period: return "Period";
            case KeyCode::Slash: return "Slash";
            case KeyCode::Semicolon: return "Semicolon";
            case KeyCode::Equal: return "Equal";
            case KeyCode::LeftBracket: return "LeftBracket";
            case KeyCode::Backslash: return "Backslash";
            case KeyCode::RightBracket: return "RightBracket";
            case KeyCode::GraveAccent: return "GraveAccent";
            case KeyCode::CapsLock: return "CapsLock";
            case KeyCode::ScrollLock: return "ScrollLock";
            case KeyCode::NumLock: return "NumLock";
            case KeyCode::PrintScreen: return "PrintScreen";
            case KeyCode::Pause: return "Pause";
            case KeyCode::Keypad0: return "Keypad0";
            case KeyCode::Keypad1: return "Keypad1";
            case KeyCode::Keypad2: return "Keypad2";
            case KeyCode::Keypad3: return "Keypad3";
            case KeyCode::Keypad4: return "Keypad4";
            case KeyCode::Keypad5: return "Keypad5";
            case KeyCode::Keypad6: return "Keypad6";
            case KeyCode::Keypad7: return "Keypad7";
            case KeyCode::Keypad8: return "Keypad8";
            case KeyCode::Keypad9: return "Keypad9";
            case KeyCode::KeypadDecimal: return "KeypadDecimal";
            case KeyCode::KeypadDivide: return "KeypadDivide";
            case KeyCode::KeypadMultiply: return "KeypadMultiply";
            case KeyCode::KeypadSubtract: return "KeypadSubtract";
            case KeyCode::KeypadAdd: return "KeypadAdd";
            case KeyCode::KeypadEnter: return "KeypadEnter";
            case KeyCode::KeypadEqual: return "KeypadEqual";
            case KeyCode::AppBack: return "AppBack";
            case KeyCode::AppForward: return "AppForward";
            case KeyCode::Oem102: return "Oem102";
            case KeyCode::GamepadStart: return "GamepadStart";
            case KeyCode::GamepadBack: return "GamepadBack";
            case KeyCode::GamepadFaceLeft: return "GamepadFaceLeft";
            case KeyCode::GamepadFaceRight: return "GamepadFaceRight";
            case KeyCode::GamepadFaceUp: return "GamepadFaceUp";
            case KeyCode::GamepadFaceDown: return "GamepadFaceDown";
            case KeyCode::GamepadDpadLeft: return "GamepadDpadLeft";
            case KeyCode::GamepadDpadRight: return "GamepadDpadRight";
            case KeyCode::GamepadDpadUp: return "GamepadDpadUp";
            case KeyCode::GamepadDpadDown: return "GamepadDpadDown";
            case KeyCode::GamepadL1: return "GamepadL1";
            case KeyCode::GamepadR1: return "GamepadR1";
            case KeyCode::GamepadL2: return "GamepadL2";
            case KeyCode::GamepadR2: return "GamepadR2";
            case KeyCode::GamepadL3: return "GamepadL3";
            case KeyCode::GamepadR3: return "GamepadR3";
            case KeyCode::GamepadLStickLeft: return "GamepadLStickLeft";
            case KeyCode::GamepadLStickRight: return "GamepadLStickRight";
            case KeyCode::GamepadLStickUp: return "GamepadLStickUp";
            case KeyCode::GamepadLStickDown: return "GamepadLStickDown";
            case KeyCode::GamepadRStickLeft: return "GamepadRStickLeft";
            case KeyCode::GamepadRStickRight: return "GamepadRStickRight";
            case KeyCode::GamepadRStickUp: return "GamepadRStickUp";
            case KeyCode::GamepadRStickDown: return "GamepadRStickDown";
        }
    }
    
    int GlfwCharCodeToImGuiKey(int charCode)
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
            case GLFW_KEY_WORLD_1:
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
            default: return ImGuiKey_None;
        }
    }
    
    KeyCode GlfwCharCodeToKeyCode(int charCode)
    {
        switch (charCode)
        {
            case GLFW_KEY_UNKNOWN: return KeyCode::None;

            // Special keys
            case GLFW_KEY_TAB: return KeyCode::Tab;
            case GLFW_KEY_LEFT: return KeyCode::LeftArrow;
            case GLFW_KEY_RIGHT: return KeyCode::RightArrow;
            case GLFW_KEY_UP: return KeyCode::UpArrow;
            case GLFW_KEY_DOWN: return KeyCode::DownArrow;
            case GLFW_KEY_PAGE_UP: return KeyCode::PageUp;
            case GLFW_KEY_PAGE_DOWN: return KeyCode::PageDown;
            case GLFW_KEY_HOME: return KeyCode::Home;
            case GLFW_KEY_END: return KeyCode::End;
            case GLFW_KEY_INSERT: return KeyCode::Insert;
            case GLFW_KEY_DELETE: return KeyCode::Delete;
            case GLFW_KEY_BACKSPACE: return KeyCode::Backspace;
            case GLFW_KEY_SPACE: return KeyCode::Space;
            case GLFW_KEY_ENTER: return KeyCode::Enter;
            case GLFW_KEY_ESCAPE: return KeyCode::Escape;

            // Modifiers
            case GLFW_KEY_LEFT_CONTROL: return KeyCode::LeftCtrl;
            case GLFW_KEY_LEFT_SHIFT: return KeyCode::LeftShift;
            case GLFW_KEY_LEFT_ALT: return KeyCode::LeftAlt;
            case GLFW_KEY_LEFT_SUPER: return KeyCode::LeftSuper;
            case GLFW_KEY_RIGHT_CONTROL: return KeyCode::RightCtrl;
            case GLFW_KEY_RIGHT_SHIFT: return KeyCode::RightShift;
            case GLFW_KEY_RIGHT_ALT: return KeyCode::RightAlt;
            case GLFW_KEY_RIGHT_SUPER: return KeyCode::RightSuper;
            case GLFW_KEY_MENU: return KeyCode::Menu;

            // Numbers (0-9)
            case GLFW_KEY_0: return KeyCode::Key0;
            case GLFW_KEY_1: return KeyCode::Key1;
            case GLFW_KEY_2: return KeyCode::Key2;
            case GLFW_KEY_3: return KeyCode::Key3;
            case GLFW_KEY_4: return KeyCode::Key4;
            case GLFW_KEY_5: return KeyCode::Key5;
            case GLFW_KEY_6: return KeyCode::Key6;
            case GLFW_KEY_7: return KeyCode::Key7;
            case GLFW_KEY_8: return KeyCode::Key8;
            case GLFW_KEY_9: return KeyCode::Key9;

            // Letters (A-Z)
            case GLFW_KEY_A: return KeyCode::A;
            case GLFW_KEY_B: return KeyCode::B;
            case GLFW_KEY_C: return KeyCode::C;
            case GLFW_KEY_D: return KeyCode::D;
            case GLFW_KEY_E: return KeyCode::E;
            case GLFW_KEY_F: return KeyCode::F;
            case GLFW_KEY_G: return KeyCode::G;
            case GLFW_KEY_H: return KeyCode::H;
            case GLFW_KEY_I: return KeyCode::I;
            case GLFW_KEY_J: return KeyCode::J;
            case GLFW_KEY_K: return KeyCode::K;
            case GLFW_KEY_L: return KeyCode::L;
            case GLFW_KEY_M: return KeyCode::M;
            case GLFW_KEY_N: return KeyCode::N;
            case GLFW_KEY_O: return KeyCode::O;
            case GLFW_KEY_P: return KeyCode::P;
            case GLFW_KEY_Q: return KeyCode::Q;
            case GLFW_KEY_R: return KeyCode::R;
            case GLFW_KEY_S: return KeyCode::S;
            case GLFW_KEY_T: return KeyCode::T;
            case GLFW_KEY_U: return KeyCode::U;
            case GLFW_KEY_V: return KeyCode::V;
            case GLFW_KEY_W: return KeyCode::W;
            case GLFW_KEY_X: return KeyCode::X;
            case GLFW_KEY_Y: return KeyCode::Y;
            case GLFW_KEY_Z: return KeyCode::Z;

            // Function keys (F1-F24)
            case GLFW_KEY_F1: return KeyCode::F1;
            case GLFW_KEY_F2: return KeyCode::F2;
            case GLFW_KEY_F3: return KeyCode::F3;
            case GLFW_KEY_F4: return KeyCode::F4;
            case GLFW_KEY_F5: return KeyCode::F5;
            case GLFW_KEY_F6: return KeyCode::F6;
            case GLFW_KEY_F7: return KeyCode::F7;
            case GLFW_KEY_F8: return KeyCode::F8;
            case GLFW_KEY_F9: return KeyCode::F9;
            case GLFW_KEY_F10: return KeyCode::F10;
            case GLFW_KEY_F11: return KeyCode::F11;
            case GLFW_KEY_F12: return KeyCode::F12;
            case GLFW_KEY_F13: return KeyCode::F13;
            case GLFW_KEY_F14: return KeyCode::F14;
            case GLFW_KEY_F15: return KeyCode::F15;
            case GLFW_KEY_F16: return KeyCode::F16;
            case GLFW_KEY_F17: return KeyCode::F17;
            case GLFW_KEY_F18: return KeyCode::F18;
            case GLFW_KEY_F19: return KeyCode::F19;
            case GLFW_KEY_F20: return KeyCode::F20;
            case GLFW_KEY_F21: return KeyCode::F21;
            case GLFW_KEY_F22: return KeyCode::F22;
            case GLFW_KEY_F23: return KeyCode::F23;
            case GLFW_KEY_F24: return KeyCode::F24;

            // Punctuation
            case GLFW_KEY_APOSTROPHE: return KeyCode::Apostrophe;
            case GLFW_KEY_COMMA: return KeyCode::Comma;
            case GLFW_KEY_MINUS: return KeyCode::Minus;
            case GLFW_KEY_PERIOD: return KeyCode::Period;
            case GLFW_KEY_SLASH: return KeyCode::Slash;
            case GLFW_KEY_SEMICOLON: return KeyCode::Semicolon;
            case GLFW_KEY_EQUAL: return KeyCode::Equal;
            case GLFW_KEY_LEFT_BRACKET: return KeyCode::LeftBracket;
            case GLFW_KEY_BACKSLASH: return KeyCode::Backslash;
            case GLFW_KEY_WORLD_1:
            case GLFW_KEY_WORLD_2: return KeyCode::Oem102;
            case GLFW_KEY_RIGHT_BRACKET: return KeyCode::RightBracket;
            case GLFW_KEY_GRAVE_ACCENT: return KeyCode::GraveAccent;

            // Lock keys
            case GLFW_KEY_CAPS_LOCK: return KeyCode::CapsLock;
            case GLFW_KEY_SCROLL_LOCK: return KeyCode::ScrollLock;
            case GLFW_KEY_NUM_LOCK: return KeyCode::NumLock;
            case GLFW_KEY_PRINT_SCREEN: return KeyCode::PrintScreen;
            case GLFW_KEY_PAUSE: return KeyCode::Pause;

            // Keypad (0-9)
            case GLFW_KEY_KP_0: return KeyCode::Keypad0;
            case GLFW_KEY_KP_1: return KeyCode::Keypad1;
            case GLFW_KEY_KP_2: return KeyCode::Keypad2;
            case GLFW_KEY_KP_3: return KeyCode::Keypad3;
            case GLFW_KEY_KP_4: return KeyCode::Keypad4;
            case GLFW_KEY_KP_5: return KeyCode::Keypad5;
            case GLFW_KEY_KP_6: return KeyCode::Keypad6;
            case GLFW_KEY_KP_7: return KeyCode::Keypad7;
            case GLFW_KEY_KP_8: return KeyCode::Keypad8;
            case GLFW_KEY_KP_9: return KeyCode::Keypad9;

            // Keypad operations
            case GLFW_KEY_KP_DECIMAL: return KeyCode::KeypadDecimal;
            case GLFW_KEY_KP_DIVIDE: return KeyCode::KeypadDivide;
            case GLFW_KEY_KP_MULTIPLY: return KeyCode::KeypadMultiply;
            case GLFW_KEY_KP_SUBTRACT: return KeyCode::KeypadSubtract;
            case GLFW_KEY_KP_ADD: return KeyCode::KeypadAdd;
            case GLFW_KEY_KP_ENTER: return KeyCode::KeypadEnter;
            case GLFW_KEY_KP_EQUAL: return KeyCode::KeypadEqual;

            // Default case: return None
            default: return KeyCode::None;
        }
    }
    
    int KeyCodeToGlfwCharCode(KeyCode keyCode)
    {
        switch (keyCode)
        {
            case KeyCode::None: return GLFW_KEY_UNKNOWN;

                // Special keys
            case KeyCode::Tab: return GLFW_KEY_TAB;
            case KeyCode::LeftArrow: return GLFW_KEY_LEFT;
            case KeyCode::RightArrow: return GLFW_KEY_RIGHT;
            case KeyCode::UpArrow: return GLFW_KEY_UP;
            case KeyCode::DownArrow: return GLFW_KEY_DOWN;
            case KeyCode::PageUp: return GLFW_KEY_PAGE_UP;
            case KeyCode::PageDown: return GLFW_KEY_PAGE_DOWN;
            case KeyCode::Home: return GLFW_KEY_HOME;
            case KeyCode::End: return GLFW_KEY_END;
            case KeyCode::Insert: return GLFW_KEY_INSERT;
            case KeyCode::Delete: return GLFW_KEY_DELETE;
            case KeyCode::Backspace: return GLFW_KEY_BACKSPACE;
            case KeyCode::Space: return GLFW_KEY_SPACE;
            case KeyCode::Enter: return GLFW_KEY_ENTER;
            case KeyCode::Escape: return GLFW_KEY_ESCAPE;

                // Modifiers
            case KeyCode::LeftCtrl: return GLFW_KEY_LEFT_CONTROL;
            case KeyCode::LeftShift: return GLFW_KEY_LEFT_SHIFT;
            case KeyCode::LeftAlt: return GLFW_KEY_LEFT_ALT;
            case KeyCode::LeftSuper: return GLFW_KEY_LEFT_SUPER;
            case KeyCode::RightCtrl: return GLFW_KEY_RIGHT_CONTROL;
            case KeyCode::RightShift: return GLFW_KEY_RIGHT_SHIFT;
            case KeyCode::RightAlt: return GLFW_KEY_RIGHT_ALT;
            case KeyCode::RightSuper: return GLFW_KEY_RIGHT_SUPER;
            case KeyCode::Menu: return GLFW_KEY_MENU;

                // Numbers (0-9)
            case KeyCode::Key0: return GLFW_KEY_0;
            case KeyCode::Key1: return GLFW_KEY_1;
            case KeyCode::Key2: return GLFW_KEY_2;
            case KeyCode::Key3: return GLFW_KEY_3;
            case KeyCode::Key4: return GLFW_KEY_4;
            case KeyCode::Key5: return GLFW_KEY_5;
            case KeyCode::Key6: return GLFW_KEY_6;
            case KeyCode::Key7: return GLFW_KEY_7;
            case KeyCode::Key8: return GLFW_KEY_8;
            case KeyCode::Key9: return GLFW_KEY_9;

                // Letters (A-Z)
            case KeyCode::A: return GLFW_KEY_A;
            case KeyCode::B: return GLFW_KEY_B;
            case KeyCode::C: return GLFW_KEY_C;
            case KeyCode::D: return GLFW_KEY_D;
            case KeyCode::E: return GLFW_KEY_E;
            case KeyCode::F: return GLFW_KEY_F;
            case KeyCode::G: return GLFW_KEY_G;
            case KeyCode::H: return GLFW_KEY_H;
            case KeyCode::I: return GLFW_KEY_I;
            case KeyCode::J: return GLFW_KEY_J;
            case KeyCode::K: return GLFW_KEY_K;
            case KeyCode::L: return GLFW_KEY_L;
            case KeyCode::M: return GLFW_KEY_M;
            case KeyCode::N: return GLFW_KEY_N;
            case KeyCode::O: return GLFW_KEY_O;
            case KeyCode::P: return GLFW_KEY_P;
            case KeyCode::Q: return GLFW_KEY_Q;
            case KeyCode::R: return GLFW_KEY_R;
            case KeyCode::S: return GLFW_KEY_S;
            case KeyCode::T: return GLFW_KEY_T;
            case KeyCode::U: return GLFW_KEY_U;
            case KeyCode::V: return GLFW_KEY_V;
            case KeyCode::W: return GLFW_KEY_W;
            case KeyCode::X: return GLFW_KEY_X;
            case KeyCode::Y: return GLFW_KEY_Y;
            case KeyCode::Z: return GLFW_KEY_Z;

                // Function keys (F1-F24)
            case KeyCode::F1: return GLFW_KEY_F1;
            case KeyCode::F2: return GLFW_KEY_F2;
            case KeyCode::F3: return GLFW_KEY_F3;
            case KeyCode::F4: return GLFW_KEY_F4;
            case KeyCode::F5: return GLFW_KEY_F5;
            case KeyCode::F6: return GLFW_KEY_F6;
            case KeyCode::F7: return GLFW_KEY_F7;
            case KeyCode::F8: return GLFW_KEY_F8;
            case KeyCode::F9: return GLFW_KEY_F9;
            case KeyCode::F10: return GLFW_KEY_F10;
            case KeyCode::F11: return GLFW_KEY_F11;
            case KeyCode::F12: return GLFW_KEY_F12;
            case KeyCode::F13: return GLFW_KEY_F13;
            case KeyCode::F14: return GLFW_KEY_F14;
            case KeyCode::F15: return GLFW_KEY_F15;
            case KeyCode::F16: return GLFW_KEY_F16;
            case KeyCode::F17: return GLFW_KEY_F17;
            case KeyCode::F18: return GLFW_KEY_F18;
            case KeyCode::F19: return GLFW_KEY_F19;
            case KeyCode::F20: return GLFW_KEY_F20;
            case KeyCode::F21: return GLFW_KEY_F21;
            case KeyCode::F22: return GLFW_KEY_F22;
            case KeyCode::F23: return GLFW_KEY_F23;
            case KeyCode::F24: return GLFW_KEY_F24;

                // Punctuation
            case KeyCode::Apostrophe: return GLFW_KEY_APOSTROPHE;
            case KeyCode::Comma: return GLFW_KEY_COMMA;
            case KeyCode::Minus: return GLFW_KEY_MINUS;
            case KeyCode::Period: return GLFW_KEY_PERIOD;
            case KeyCode::Slash: return GLFW_KEY_SLASH;
            case KeyCode::Semicolon: return GLFW_KEY_SEMICOLON;
            case KeyCode::Equal: return GLFW_KEY_EQUAL;
            case KeyCode::LeftBracket: return GLFW_KEY_LEFT_BRACKET;
            case KeyCode::Backslash: return GLFW_KEY_BACKSLASH;
            case KeyCode::Oem102: return GLFW_KEY_WORLD_1;
            case KeyCode::RightBracket: return GLFW_KEY_RIGHT_BRACKET;
            case KeyCode::GraveAccent: return GLFW_KEY_GRAVE_ACCENT;

                // Lock keys
            case KeyCode::CapsLock: return GLFW_KEY_CAPS_LOCK;
            case KeyCode::ScrollLock: return GLFW_KEY_SCROLL_LOCK;
            case KeyCode::NumLock: return GLFW_KEY_NUM_LOCK;
            case KeyCode::PrintScreen: return GLFW_KEY_PRINT_SCREEN;
            case KeyCode::Pause: return GLFW_KEY_PAUSE;

                // Keypad (0-9)
            case KeyCode::Keypad0: return GLFW_KEY_KP_0;
            case KeyCode::Keypad1: return GLFW_KEY_KP_1;
            case KeyCode::Keypad2: return GLFW_KEY_KP_2;
            case KeyCode::Keypad3: return GLFW_KEY_KP_3;
            case KeyCode::Keypad4: return GLFW_KEY_KP_4;
            case KeyCode::Keypad5: return GLFW_KEY_KP_5;
            case KeyCode::Keypad6: return GLFW_KEY_KP_6;
            case KeyCode::Keypad7: return GLFW_KEY_KP_7;
            case KeyCode::Keypad8: return GLFW_KEY_KP_8;
            case KeyCode::Keypad9: return GLFW_KEY_KP_9;

                // Keypad operations
            case KeyCode::KeypadDecimal: return GLFW_KEY_KP_DECIMAL;
            case KeyCode::KeypadDivide: return GLFW_KEY_KP_DIVIDE;
            case KeyCode::KeypadMultiply: return GLFW_KEY_KP_MULTIPLY;
            case KeyCode::KeypadSubtract: return GLFW_KEY_KP_SUBTRACT;
            case KeyCode::KeypadAdd: return GLFW_KEY_KP_ADD;
            case KeyCode::KeypadEnter: return GLFW_KEY_KP_ENTER;
            case KeyCode::KeypadEqual: return GLFW_KEY_KP_EQUAL;

                // Not represented by GLFW keyboard key codes.
            case KeyCode::AppBack:
            case KeyCode::AppForward:
            case KeyCode::GamepadBack:
            case KeyCode::GamepadDpadDown:
            case KeyCode::GamepadDpadLeft:
            case KeyCode::GamepadDpadRight:
            case KeyCode::GamepadDpadUp:
            case KeyCode::GamepadFaceDown:
            case KeyCode::GamepadFaceLeft:
            case KeyCode::GamepadFaceRight:
            case KeyCode::GamepadFaceUp:
            case KeyCode::GamepadL1:
            case KeyCode::GamepadL2:
            case KeyCode::GamepadL3:
            case KeyCode::GamepadLStickDown:
            case KeyCode::GamepadLStickLeft:
            case KeyCode::GamepadLStickRight:
            case KeyCode::GamepadLStickUp:
            case KeyCode::GamepadR1:
            case KeyCode::GamepadR2:
            case KeyCode::GamepadR3:
            case KeyCode::GamepadRStickDown:
            case KeyCode::GamepadRStickLeft:
            case KeyCode::GamepadRStickRight:
            case KeyCode::GamepadRStickUp:
            case KeyCode::GamepadStart:
                return GLFW_KEY_UNKNOWN;
        }

        return GLFW_KEY_UNKNOWN;
    }
}
