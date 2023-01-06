#pragma once

#include <utility>
#include <string>

#include <SDL3/SDL.h>

namespace wv
{
	/// @brief Data container for the properties used to initialize an SDL window
	struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		float AspectRatio;
		std::pair<unsigned int, unsigned int> MaxDimensions;
		std::pair<unsigned int, unsigned int> MinDimensions;

		WindowProperties(const std::string& title = "wave.",
			unsigned int width = 1250,
			unsigned int height = 725,
			unsigned int maxWidth = 3840,
			unsigned int maxHeight = 2160,
			unsigned int minWidth = 500,
			unsigned int minHeight = 300)
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
		void Init();

		/// @brief Render loop for SDL window 
		void Render();

		/// @brief Destroys window in SDL and ends SDL session
		void Teardown();
	
	private:
		WindowProperties m_WindowProperties;
		SDL_Window* m_Window = nullptr;
	};
}
