#include "Application.h"

#include "Timer.h"

#include <surf/surf.h>
#include <surf/SymbolTable.h>

#include <map>

void fun(surf_FunArg a, ...) { std::cout << "wtf this worked... " << a << std::endl; }

void test(const char* aStr, ...) { std::cout << "another wtf " << aStr << std::endl; }
void testTwo(const char* another, ...) { std::cout << "last one " << another << std::endl; }
void testThree(const char* another, ...) { std::cout << "last one again " << another << std::endl; }

wv::Application::Application()
	: m_Window(new Window)
{
	core::Log(ELogType::Debug, "Starting surf bridge...");

	surf_ApiResult result = surf_StartBridge();
	WAVE_ASSERT(result != SURF_API_ERROR, "Failed to connect to the surf API");

	// Example line of surf
	const char* buffer = "let x: v2 = (1, 2);";
	char* out = surf_InterpLine(buffer);
	core::Log(ELogType::Debug, "Sent: {}", buffer);
	core::Log(ELogType::Debug, "Received: {}", out);
	surf_InterpDestroyLine(out);

	// Symbol registration
	surf_InterpRegisterSymbol("myFunc", &fun);
	const char* testBuffer = "ref myFunc();";
	char* reflectResult = surf_InterpLine(testBuffer);
	core::Log(ELogType::Debug, "Result: {}", reflectResult);
	surf_InterpDestroyLine(reflectResult);

	result = surf_DestroyBridge();
	WAVE_ASSERT(result != SURF_API_ERROR, "Failed to destroy surf bridge");
}

wv::Application::~Application()
{
	delete m_Window;
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
	m_Renderer = Renderer::CreateRendererWithGAPI(m_Window, gapi);
	m_Renderer->Init();

	bool isRunning = true;
	while (isRunning)
	{
		// Tick delta time
		core::Timer::Tick();

		// Handle polled events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				isRunning = false;

			if (event.type == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_Window->GetSDLWindow()))
				isRunning = false;
		}

		// Wait events if the window is minimized
		while (m_Window->IsMinimized())
		{
			SDL_WaitEvent(&event);
		}

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
