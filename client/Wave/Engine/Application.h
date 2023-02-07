#pragma once

#include "Window.h"
#include "Renderer.h"

namespace wv
{
	class Application
	{
	public:
		Application();
		~Application();

		/// @brief Runs the application and initializes the renderer
		void Run();

		/// @brief Performs application teardown logic
		void Teardown() const;

	private:
		Window* m_Window = nullptr;
		core::Unique<Renderer> m_VkRenderer = nullptr;
	};
}
