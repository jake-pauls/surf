#pragma once

#include "Window.h"

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
		void Teardown();

	private:
		Window* m_Window = nullptr;
	};
}