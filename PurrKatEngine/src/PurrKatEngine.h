#pragma once

#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/Logs/Log.h"
#include <cstdio>

// --- Entry Point ---
#include "PurrKatEngine/EntryPoint.h"

// --- USER ENTRY POINT MACROS ---
// Classic usage: CREATE_APPLICATION { auto app = new MyGameClass(); // Optional prep...; return app; }
#define CREATE_APPLICATION PurrKatEngine::Application* PurrKatEngine::CreateApplication()
// For a quick start if there is no need for extra customisation.
#define CREATE_APPLICATION_FROM_CLASS(class) PurrKatEngine::Application* PurrKatEngine::CreateApplication() { return new class(); }