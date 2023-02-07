#pragma once

#include <SDL3/SDL.h>

#include "Renderer.h"

namespace wv
{
	class Window;

	/// @brief Abstract renderer context for a particular API implementation
	class RendererContext
	{
	public:
		virtual ~RendererContext() = default;

		/// @brief Initializes the renderer's particular context
		virtual void Init() = 0;

		/// @brief Retrieves context-specific SDL window flags
		virtual SDL_WindowFlags GetContextSDLWindowFlags() const = 0;

		/// @brief Creates a context for a provided renderer to a window
		/// @return Unique pointer containing the graphics context 
		static core::Unique<RendererContext> CreateRendererContext(Window* window, Renderer::GraphicsAPI api);
	};
}
