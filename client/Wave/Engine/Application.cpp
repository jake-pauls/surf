#include "Application.h"

#include "Window.h"
#include "Camera.h"
#include "ToolPanel.h"
#include "SurfEngine.h"
#include "SurfStore.h"
#include "Renderer.h"
#include "Vulkan/VkRenderer.h"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <surf/surf.h>

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

	// Serve the surf configuration's rendering API
	surf_Cfg cfg = SurfEngine::GetSurfCfg();

	// Initialize the window and it's corresponding graphics context
	m_Window->Init(cfg.Gapi);
	m_Renderer = Renderer::CreateRendererWithSurfCfg(m_Window, m_Camera, cfg);
	m_Renderer->Init();

	// Initialize the tool panel
	m_ToolPanel = new ToolPanel(cfg.Gapi);

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

		SurfStore::Update();

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
