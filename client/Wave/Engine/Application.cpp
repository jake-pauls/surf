#include "Application.h"

#include "Window.h"
#include "Camera.h"
#include "Renderer.h"

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
	//core::Log(ELogType::Debug, "Starting surf bridge...");
	//surf_ApiResult result = surf_StartBridge();
	//WAVE_ASSERT(result != SURF_API_ERROR, "Failed to connect to the surf API");

	//// Config definition
	//std::string scriptDir = core::FileSystem::GetScriptsDirectory().string();
	//surf_Cfg cfg = surf_CfgLoad(scriptDir.c_str());
	//if (surf_CfgIsNil(&cfg))
	//	core::Log(ELogType::Warn, "Bitch its nil");

	//// Example line of surf
	//const char* buffer = "let x: v2 = (1, 2);";
	//char* out = surf_InterpLine(buffer);
	//core::Log(ELogType::Debug, "Sent: {}", buffer);
	//core::Log(ELogType::Debug, "Received: {}", out);
	//surf_InterpFreeString(out);

	//// Symbol registration
	//surf_InterpRegisterSymbol("myFunc", (surf_fun_t) &Fun);
	//const char* testBuffer = "ref myFunc();";
	//char* reflectResult = surf_InterpLine(testBuffer);
	//surf_InterpFreeString(reflectResult);

	//surf_InterpRegisterSymbol("anotherFun", (surf_fun_t) &AnotherFun);
	//const char* anotherTestBuffer = "ref anotherFun(1:int, 2.02:flt, (1,2):v2, (1,2,3):v3, (1.23,2.345,34.54,439.2):v4, \"test\":str);";
	//char* anotherReflectResult = surf_InterpLine(anotherTestBuffer);
	//surf_InterpFreeString(anotherReflectResult);

	//surf_InterpBindInt("anInt", 12345);
	//surf_InterpBindFlt("aFlt", (float) 1.2034);
	//surf_InterpBindStr("aStr", "strstr");

	//surf_InterpBindV2("aV2", 1.02f, 2.02f);
	//surf_InterpBindV3("aV3", 1.02f, 2.02f, 3.02f);
	//surf_InterpBindV4("aV4", 1.02f, 2.02f, 3.02f, 4.02f);

	//int anInt;
	//if (surf_InterpGetInt("anInt", &anInt))
	//	core::Log(ELogType::Warn, "anInt: {}", anInt);

	//float aFlt;
	//if (surf_InterpGetFlt("aFlt", &aFlt))
	//	core::Log(ELogType::Warn, "aFlt: {}", aFlt);

	//char* aStr;
	//if (surf_InterpGetStr("aStr", &aStr))
	//	core::Log(ELogType::Warn, "aStr: {}", aStr);
	//surf_InterpFreeString(aStr);

	//surf_V2 aV2;
	//if (surf_InterpGetV2("aV2", &aV2))
	//	core::Log(ELogType::Warn, "aV2: x->{} y->{}", aV2.x, aV2.y);

	//surf_V3 aV3;
	//if (surf_InterpGetV3("aV3", &aV3))
	//	core::Log(ELogType::Warn, "aV3: x->{} y->{} z->{}", aV3.x, aV3.y, aV3.z);

	//surf_V4 aV4;
	//if (surf_InterpGetV4("aV4", &aV4))
	//	core::Log(ELogType::Warn, "aV4: x->{} y->{} z->{} w->{}", aV4.x, aV4.y, aV4.z, aV4.w);

	//result = surf_DestroyBridge();
	//WAVE_ASSERT(result != SURF_API_ERROR, "Failed to destroy surf bridge");
}

wv::Application::~Application()
{
	delete m_Window;
	delete m_Camera;
}

void wv::Application::Run()
{
	core::Log(ELogType::Info, "[Application] Starting the wave");

	// Create rendering context, for now it's Vulkan
	// TODO: Determining this at runtime would be sick
	using GAPI = Renderer::GraphicsAPI;
	GAPI gapi = GAPI::Vulkan;

	// Initialize the window and it's corresponding graphics context
	m_Window->Init(gapi);
	m_Renderer = Renderer::CreateRendererWithGAPI(m_Window, m_Camera, gapi);
	m_Renderer->Init();

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
	
		if (gapi == GAPI::Vulkan)
			ImGui_ImplVulkan_NewFrame();
		else
			WAVE_ASSERT(false, "Graphics API not configured with ImGui");

		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();

		m_Renderer->Draw();
	}

	Teardown();
}

void wv::Application::Teardown() const
{
	core::Log(ELogType::Info, "[Application] Tearing down the wave");

	m_Renderer->Teardown();

	m_Window->Teardown();
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
