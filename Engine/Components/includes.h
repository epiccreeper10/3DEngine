#pragma once
#include "Input/inputSystem.h"
#include "Audio/audioSystem.h"
#include "Renderer/renderer.h"
#include "Resource/resourceManager.h"
#include "Core/time.h"
#include "Framework/component.h"
#include "Physics/physicsSystem.h"
#include "Framework/EventManager.h"

namespace neu
{
	extern InputSystem g_inputSystem;
	extern Renderer g_renderer;
	extern Time g_time;
	extern AudioSystem g_audioSystem;
	extern ResourceManager g_resourceManager;
	extern PhysicsSystem g_physicsSystem;
	extern EventManager g_eventManager;
}