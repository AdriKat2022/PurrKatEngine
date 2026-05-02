#pragma once

namespace PurrKatEngine {

    // Engine-level key codes. These are independent from any 3rd-party library
    // (such as ImGui or GLFW). Keep this API stable so clients only need to
    // include the engine headers.
    enum class KeyCode : std::uint8_t
    {
        None = 0,
        Tab, LeftArrow, RightArrow, UpArrow, DownArrow,
        PageUp, PageDown, Home, End, Insert, Delete, Backspace, Space,
        Enter, Escape,
        LeftCtrl, LeftShift, LeftAlt, LeftSuper,
        RightCtrl, RightShift, RightAlt, RightSuper,
        Menu, Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,

        // Letters (Aligned with ASCII)
        A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
        Apostrophe, Comma, Minus, Period, Slash, Semicolon, Equal,
        LeftBracket, Backslash, RightBracket, GraveAccent, CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
        Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
        KeypadDecimal, KeypadDivide,
        KeypadMultiply, KeypadSubtract, KeypadAdd, KeypadEnter, KeypadEqual, AppBack, AppForward, Oem102,

        // Gamepad
        GamepadStart, GamepadBack, GamepadFaceLeft, GamepadFaceRight, GamepadFaceUp, GamepadFaceDown, GamepadDpadLeft, GamepadDpadRight, GamepadDpadUp, GamepadDpadDown,
        GamepadL1, GamepadR1, GamepadL2, GamepadR2, GamepadL3, GamepadR3,
        GamepadLStickLeft, GamepadLStickRight, GamepadLStickUp, GamepadLStickDown, GamepadRStickLeft,
        GamepadRStickRight, GamepadRStickUp, GamepadRStickDown,
    };

    enum class MouseButtonCode : byte
    {
        MouseLeft         = 0,
        MouseRight        = 1,
        MouseMiddle       = 2,
        MouseButton4      = 3,
        MouseButton5      = 4,
        MouseButton6      = 5,
        MouseButton7      = 6,
        MouseButton8      = 7,
        MouseButtonLast   = MouseButton8,
        MouseButtonLeft   = MouseLeft,
        MouseButtonRight  = MouseRight,
        MouseButtonMiddle = MouseMiddle
    };
    
    const char* to_string(KeyCode keyCode);

    // Low-level helpers used by the platform and ImGui integration. These
    // functions intentionally avoid exposing ImGui types in the public
    // header. `GlfwCharCodeToImGuiKey` returns the integer value that
    // corresponds to ImGui's ImGuiKey enum; callers that need the actual
    // ImGuiKey should include ImGui headers and cast the returned int.
    int GlfwCharCodeToImGuiKey(int charCode);
    KeyCode GlfwCharCodeToKeyCode(int charCode);
    int KeyCodeToGlfwCharCode(KeyCode keyCode);
}


