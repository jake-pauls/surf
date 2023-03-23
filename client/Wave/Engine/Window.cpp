#include "Window.h"

#include <surf/surf.h>

wv::Window::Window(const WindowProperties& props)
	: m_WindowProperties(props)
{
	core::Log(ELogType::Info, "[Window] Creating a window [{} - {} x {}]", m_WindowProperties.Title, m_WindowProperties.Width, m_WindowProperties.Height);

	int result = SDL_Init(SDL_INIT_VIDEO);
	WAVE_ASSERT(result >= 0, "Failed to initialize SDL context");
}

void wv::Window::Init(surf_Gapi gapi)
{
	WAVE_ASSERT(!m_Window, "Init() is being called on a window that has already been initialized");

	m_RendererContext = RendererContext::CreateRendererContext(this, gapi);
	WAVE_ASSERT(m_RendererContext, "Failed to create renderer context");
	m_RendererContext->Init();
	
	// Set window flags, retrieve context specific flags from render context
	SDL_WindowFlags contextFlags = m_RendererContext->GetContextSDLWindowFlags();
	SDL_WindowFlags windowFlags = static_cast<SDL_WindowFlags>(contextFlags | SDL_WINDOW_RESIZABLE);

	m_Window = SDL_CreateWindow(
		m_WindowProperties.Title.c_str(), 
		m_WindowProperties.Width, 
		m_WindowProperties.Height, 
		windowFlags
	);
	WAVE_ASSERT(m_Window, "Failed to create SDL window");
	
	SDL_SetWindowMaximumSize(m_Window, m_WindowProperties.MaxDimensions.first, m_WindowProperties.MaxDimensions.second);
	SDL_SetWindowMinimumSize(m_Window, m_WindowProperties.MinDimensions.first, m_WindowProperties.MinDimensions.second);
}

void wv::Window::Render() const
{
	// Draw stuff here
}

void wv::Window::Teardown() const
{
	WAVE_ASSERT(m_Window, "Teardown() is being called on a Window that hasn't been initialized");
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}
