#include "Window.h"

wv::Window::Window(const WindowProperties& props)
	: m_WindowProperties(props)
{
	int result = SDL_Init(SDL_INIT_VIDEO);
	WAVE_ASSERT(result >= 0, "Failed to initialize SDL context");

	// Initialize Vulkan specific SDL settings?
}

void wv::Window::Init()
{
	WAVE_ASSERT(!m_Window, "Init() is being called on a window that has already been initialized");
	SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	m_Window = SDL_CreateWindow(
		m_WindowProperties.Title.c_str(), 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		m_WindowProperties.Width, 
		m_WindowProperties.Height, 
		flags
	);
	WAVE_ASSERT(m_Window, "Failed to create SDL window");

	SDL_SetWindowMaximumSize(m_Window, m_WindowProperties.MaxDimensions.first, m_WindowProperties.MaxDimensions.second);
	SDL_SetWindowMinimumSize(m_Window, m_WindowProperties.MinDimensions.first, m_WindowProperties.MinDimensions.second);
}

void wv::Window::Render()
{
	// Vulkan-specific updates for the SDL window
}

void wv::Window::Teardown()
{
	WAVE_ASSERT(m_Window, "Teardown() is being called on a Window that hasn't been initialized")
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}
