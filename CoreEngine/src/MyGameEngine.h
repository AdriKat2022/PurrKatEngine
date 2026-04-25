#pragma once

#include "MyGameEngine/Application.h"
#include <stdio.h>

// Entry Point
#include "MyGameEngine/EntryPoint.h"

// Classic usage: CREATE_APPLICATION { auto app = new MyGameClass(); // Optional prep...; return app; }
#define CREATE_APPLICATION MyGameEngine::Application* MyGameEngine::CreateApplication()

// For a quick start if there is no need for extra customisation.
#define CREATE_APPLICATION_FROM_CLASS(class) MyGameEngine::Application* MyGameEngine::CreateApplication() { return new class(); }