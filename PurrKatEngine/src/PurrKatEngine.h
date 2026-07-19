#pragma once

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/Components/Standard2DInputController.h"
#include "PurrKatEngine/Components/Transform.h"
#include "PurrKatEngine/ImGui/ImGuiLayer.h"
#include "PurrKatEngine/Inputs/Input.h"
#include "PurrKatEngine/Inputs/KeyCodes.h"
#include "PurrKatEngine/Inputs/Time.h"
#include "PurrKatEngine/Logs/Log.h"
#include "PurrKatEngine/Profiling/Profiler.h"
#include "PurrKatEngine/Renderer/OrthographicCamera.h"
#include "PurrKatEngine/Renderer/OrthographicCameraController.h"
#include "PurrKatEngine/Renderer/RenderCommand.h"
#include "PurrKatEngine/Renderer/Renderer.h"
#include "PurrKatEngine/Renderer/Texture.h"
#include "PurrKatEngine/Renderer/VertexArray.h"
#include "PurrKatEngine/Renderer/Renderer2D/Renderer2D.h"
#include "PurrKatEngine/Utility/EventAction.h"
#include "PurrKatEngine/Utility/ImGuiUtility.h"

// --- USER ENTRY POINT MACROS --- 
// You will need to separately include the "EntryPoint.h" header file in your main.cpp file, preferably where you use these macros.

// QUICK START
// The application launches an instance of the provided class that extends PKE::Application.
// Ex: RUN_CLASS(MyClassThatExtendsPKEApplication)
#define RUN_CLASS(class) PurrKatEngine::Application* PurrKatEngine::CreateApplication() { return new class(); }

// --- OR ---

// CUSTOM START
// The application launches an instance of the returned class that extends PKE::Application.
// Ex: RUN_APPLICATION { auto app = new MyClassThatExtendsPKEApplication(); // You optional prep... ; return app; }
#define RUN_APPLICATION PurrKatEngine::Application* PurrKatEngine::CreateApplication()

// -------------------------------

namespace PKE = PurrKatEngine;