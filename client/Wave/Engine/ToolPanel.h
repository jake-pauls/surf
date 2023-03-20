#pragma once

#include <functional>

#include "Renderer.h"

namespace wv
{
	class ToolPanel
	{
	public:
		explicit ToolPanel(Renderer::GraphicsAPI gapi);
		~ToolPanel() = default;

		/// @brief Draws ImGui tools to the SDL window
		void Draw();

		/// @brief String for the model currently selected in the tool panel
		static std::string s_SelectedModel;

	private:
		std::function<void(void)> m_ImGuiNewFrameFunction;
	};
}