#pragma once

#include <SDL3/SDL.h>

#include "IRenderer.h"

namespace wv
{
	class Window;

	/// @brief Abstract renderer context for a particular API implementation
	class IRendererContext
	{
	public:
		virtual ~IRendererContext() = default;

		/// @brief Initializes the renderer's particular context
		virtual void Init() = 0;

		/// @brief Retrieves context-specific SDL window flags
		virtual SDL_WindowFlags GetContextSDLWindowFlags() = 0;

		/// @brief Creates a context for a provided renderer to a window
		/// @return Unique pointer containing the graphics context 
		static Unique<IRendererContext> CreateRendererContext(Window* window, IRenderer::GraphicsAPI api);
	};
}