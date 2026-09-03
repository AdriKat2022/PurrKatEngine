#pragma once
#include "PurrKatEngine/Inputs/Keys.h"

namespace PurrKatEngine::Controls
{
    // ------------ Editor Camera Controls ------------
    // Mouse Movements
    inline constexpr KeyCode CAMERA_MODIFIER_KEY = KeyCode::LeftAlt; // Hold this key to enable camera controls, none means no modifier key is required.
    inline constexpr MouseButtonCode CAMERA_PAN_BUTTON = MouseButtonCode::MouseMiddle;
    inline constexpr MouseButtonCode CAMERA_ROTATE_BUTTON = MouseButtonCode::MouseLeft;
    inline constexpr MouseButtonCode CAMERA_ZOOM_BUTTON = MouseButtonCode::MouseRight;
    
    inline constexpr KeyCode CAMERA_REFOCUS_KEY = KeyCode::F; // Press this key to refocus the camera on the selected entity.
    
    // Movement
    inline constexpr bool CAMERA_ENABLE_MOVEMENT = true; // Enable or disable camera movement with WASD and QE keys.
    inline constexpr KeyCode CAMERA_UP_KEY = KeyCode::E;
    inline constexpr KeyCode CAMERA_DOWN_KEY = KeyCode::Q;
    inline constexpr KeyCode CAMERA_FORWARD_KEY = KeyCode::W;
    inline constexpr KeyCode CAMERA_LEFT_KEY = KeyCode::A;
    inline constexpr KeyCode CAMERA_BACK_KEY = KeyCode::S;
    inline constexpr KeyCode CAMERA_RIGHT_KEY = KeyCode::D;
    
    // ----------- Editor Controls ------------
    // Mouse picking
    inline constexpr MouseButtonCode MOUSE_PICK_BUTTON = MouseButtonCode::MouseLeft;
    
    // Gizmos Shortcuts
    inline constexpr KeyCode GIZMOS_TRANSLATE_KEY = KeyCode::W;
    inline constexpr KeyCode GIZMOS_ROTATE_KEY = KeyCode::E;
    inline constexpr KeyCode GIZMOS_SCALE_KEY = KeyCode::R;
};
