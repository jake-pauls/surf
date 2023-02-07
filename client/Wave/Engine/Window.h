#pragma once

#include <utility>
#include <string>

#include <SDL3/SDL.h>

#include "RendererContext.h"

namespace wv
{
	/// @brief Data container for the properties used to initialize an SDL window
	struct WindowProperties
	{
		std::string Title;
		const uint32_t Width;
		const uint32_t Height;
		const float AspectRatio;
		const std::pair<uint32_t, uint32_t> MaxDimensions;
		const std::pair<uint32_t, uint32_t> MinDimensions;

		WindowProperties(const std::string& title = "wave.",
			const uint32_t width = 1250,
			const uint32_t height = 725,
			const uint32_t maxWidth = 3840,
			const uint32_t maxHeight = 2160,
			const uint32_t minWidth = 500,
			const uint32_t minHeight = 300)
			: Title(title)
			, Width(width)
			, Height(height)
			, AspectRatio(static_cast<float>(width) / static_cast<float>(height))
			, MaxDimensions(maxWidth, maxHeight)
			, MinDimensions(minWidth, minHeight)
		{
		}
	};

	class Window
	{
	public:
		explicit Window(const WindowProperties& props = WindowProperties());

		/// @brief Initializes the window in SDL, sets default settings from passed properties 
		void Init(Renderer::GraphicsAPI gapi);

		/// @brief Render loop for SDL window 
		void Render() const;

		/// @brief Destroys window in SDL and ends SDL session
		void Teardown() const;
	
		/// @brief Retrievies pointer to the currently open SDL window
		inline SDL_Window* GetSDLWindow() const { return m_Window; }

		/// @brief Retrieves pointer to the SDL windows current renderer context 
		inline RendererContext* GetRendererContext() const { return m_RendererContext.get(); }

	private:
		WindowProperties m_WindowProperties;
		SDL_Window* m_Window = nullptr;
		core::Unique<RendererContext> m_RendererContext = nullptr;
	};
}
