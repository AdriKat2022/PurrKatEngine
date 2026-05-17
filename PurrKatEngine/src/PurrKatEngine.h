#pragma once

#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/Logs/Log.h"
#include "PurrKatEngine/ImGui/ImGuiLayer.h"

#include "PurrKatEngine/Inputs/Input.h"
#include "PurrKatEngine/Inputs/KeyCodes.h"
#include "PurrKatEngine/Inputs/Time.h"

#include "PurrKatEngine/Renderer/Renderer.h"
#include "PurrKatEngine/Renderer/RenderCommand.h"
#include "PurrKatEngine/Renderer/OrthographicCamera.h"
#include "PurrKatEngine/Renderer/VertexArray.h"

#include "PurrKatEngine/Components/Standard2DInputController.h"

// --- Entry Point ---
#include "PurrKatEngine/EntryPoint.h"

// --- USER ENTRY POINT MACROS ---
// Classic usage: CREATE_APPLICATION { auto app = new MyGameClass(); // Optional prep...; return app; }
#define CREATE_APPLICATION PurrKatEngine::Application* PurrKatEngine::CreateApplication()
// For a quick start if there is no need for extra customisation.
#define CREATE_APPLICATION_FROM_CLASS(class) PurrKatEngine::Application* PurrKatEngine::CreateApplication() { return new class(); }