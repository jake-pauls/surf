#include "Application.h"

#include "Window.h"
#include "Camera.h"
#include "ToolPanel.h"
#include "SurfEngine.h"
#include "Renderer.h"
#include "Vulkan/VkRenderer.h"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <surf/surf.h>

void Fun(surf_argpack_t argpack) 
{ 
	core::Log(ELogType::Debug, "fun(): a simple function callback");
}

void AnotherFun(surf_argpack_t argpack) 
{ 
	int aInt = surf_ArgpackGetInt(argpack, 0);
	float aFlt = surf_ArgpackGetFlt(argpack, 1);
	surf_V2 v2 = surf_ArgpackGetV2(argpack, 2);
	surf_V3 v3 = surf_ArgpackGetV3(argpack, 3);
	surf_V4 v4 = surf_ArgpackGetV4(argpack, 4);

	// TODO: Strings can only be placed as the last arg of an argpack 
	//		 Presumably for alignment reasons?
	char* aStr = surf_ArgpackGetStr(argpack, 5);

	core::Log(ELogType::Debug, "anotherFun(): a function callback with an argpack");
	core::Log(ELogType::Debug, "int is {}", aInt);
	core::Log(ELogType::Debug, "float is {}", aFlt);
	core::Log(ELogType::Debug, "string is {}", aStr);
	core::Log(ELogType::Debug, "v2 is (x:{}, y:{})", v2.x, v2.y);
	core::Log(ELogType::Debug, "v3 is (x:{}, y:{}, z:{})", v3.x, v3.y, v3.z);
	core::Log(ELogType::Debug, "v4 is (x:{}, y:{}, z:{}, w:{})", v4.x, v4.y, v4.z, v4.w);
}

//

wv::Application::Application()
	: m_Window(new Window)
	, m_Camera(new Camera)
{
}

wv::Application::~Application()
{
	delete m_Window;
	delete m_Camera;
	delete m_ToolPanel;
}

void wv::Application::Run()
{
	core::Log(ELogType::Info, "[Application] Starting the wave");

	// Start the surf runner
	SurfEngine::Start();

	// Example line of surf
	const char* buffer = "let x: v2 = (1, 2);";
	std::string out = SurfEngine::InterpLine(buffer);
	core::Log(ELogType::Debug, "Sent: {}", buffer);
	core::Log(ELogType::Debug, "Received: {}", out);

	if (SurfEngine::InterpFile("demo.surf"))
		core::Log(ELogType::Debug, "Interpreted demo.surf!");

	// Symbol registration/unregistration
	SurfEngine::RegisterFunction("myFunc", (surf_fun_t) &Fun);
	SurfEngine::InterpLine("ref myFunc();");
	SurfEngine::DeregisterFunction("myFunc");

	SurfEngine::BindInt("anInt", 12345);
	SurfEngine::BindFlt("aFlt", 1.2345f);
	SurfEngine::BindStr("aStr", "SurfEngine bound this str");
	SurfEngine::BindV2("aV2", { 2.02f, 2.02f });

	int anInt = SurfEngine::GetInt("anInt");
	core::Log(ELogType::Warn, "anInt: {}", anInt);

	float aFlt = SurfEngine::GetFlt("aFlt");
	core::Log(ELogType::Warn, "aFlt: {}", aFlt);

	std::string aStr = SurfEngine::GetStr("aStr");
	core::Log(ELogType::Warn, "aStr: {}", aStr);

	glm::vec2 aV2 = SurfEngine::GetV2("aV2");
	core::Log(ELogType::Warn, "aV2: x -> {}  y -> {}", aV2.x, aV2.y);

	// Serve the surf configuration's rendering API
	Renderer::GraphicsAPI gapi;
	surf_Cfg cfg = SurfEngine::GetSurfCfg();
	switch (cfg.Gapi)
	{
	case SURF_GAPI_VULKAN:
		gapi = Renderer::GraphicsAPI::Vulkan;
		break;
	case SURF_GAPI_DIRECTX:
		gapi = Renderer::GraphicsAPI::DirectX;
		break;
	case SURF_GAPI_OPENGL:
		gapi = Renderer::GraphicsAPI::OpenGL;
		break;
	case SURF_GAPI_NIL:
		WAVE_ASSERT(false, "surf's configuration reported an unknown graphics API");
		break;
	}

	// Initialize the window and it's corresponding graphics context
	m_Window->Init(gapi);
	m_Renderer = Renderer::CreateRendererWithGAPI(m_Window, m_Camera, gapi);
	m_Renderer->Init();

	// Initialize the tool panel
	m_ToolPanel = new ToolPanel(gapi);

	// Initialize SDL Keys
	bool sdlKeys[322];
	for (size_t i = 0; i < 322; ++i)
		sdlKeys[i] = false;

	bool isRunning = true;
	while (isRunning)
	{
		// Tick delta time
		core::Timer::Tick();

		// Handle polled events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);

			if (event.type == SDL_EVENT_QUIT)
				isRunning = false;

			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(m_Window->GetSDLWindow()))
				isRunning = false;

			switch (event.type)
			{
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				if (event.button.button == SDL_BUTTON_MIDDLE)
					m_IsMouseDown = true;
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				if (event.button.button == SDL_BUTTON_MIDDLE)
					m_IsMouseDown = false;
				break;
			case SDL_EVENT_MOUSE_WHEEL:
				m_Camera->Zoom(event.wheel.y);
				break;
			case SDL_EVENT_KEY_DOWN:
				sdlKeys[event.key.keysym.scancode] = true;
				break;
			case SDL_EVENT_KEY_UP:
				sdlKeys[event.key.keysym.scancode] = false;
				break;
			}

			UpdateControls(sdlKeys);
		}

		// Wait events if the window is minimized
		while (m_Window->IsMinimized())
		{
			SDL_WaitEvent(&event);
		}
	
		m_ToolPanel->Draw();
		m_Renderer->Draw();
	}

	Teardown();
}

void wv::Application::Teardown() const
{
	core::Log(ELogType::Info, "[Application] Tearing down the wave");

	m_Renderer->Teardown();

	m_Window->Teardown();

	SurfEngine::Destroy();
}

void wv::Application::UpdateControls(bool* keys)
{
	// Use escape to toggle the mouse being locked
	if (keys[SDL_SCANCODE_ESCAPE])
	{
		m_IsMouseLocked = !m_IsMouseLocked;
		SDL_SetRelativeMouseMode(static_cast<SDL_bool>(m_IsMouseLocked));
	}

	// Handle mouse orbits if mouse is locked
	if (m_IsMouseDown && m_IsMouseLocked)
	{
		float xPos, yPos;
		SDL_GetMouseState(&xPos, &yPos);

		if (m_FirstFrame)
		{
			m_LastX = xPos;
			m_LastY = yPos;
			m_FirstFrame = false;
		}

		float xOffset = xPos - m_LastX;
		float yOffset = m_LastY - yPos;
		m_LastX = xPos;
		m_LastY = yPos;

		m_Camera->Orbit(xOffset, yOffset);
	}
}
