#include "Application.h"

wv::Application::Application()
	: m_Window(new Window)
{
}

wv::Application::~Application()
{
	delete m_Window;
}

void wv::Application::Run()
{
	core::Log(ELogType::Info, "[Application] Starting the wave");

	m_Window->Init();

	m_VkRenderer = Renderer::CreateRendererWithAPISpec(wv::Renderer::GraphicsAPI::Vulkan);
	m_VkRenderer->Init();

	bool isRunning = true;
	while (isRunning)
	{
		// Handle polled events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				isRunning = false;

			if (event.type == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_Window->GetSDLWindow()))
				isRunning = false;
		}
	}

	Teardown();
}

void wv::Application::Teardown() const
{
	core::Log(ELogType::Info, "[Application] Tearing down the wave");
}
