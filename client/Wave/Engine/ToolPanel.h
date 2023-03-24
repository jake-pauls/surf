#pragma once

#include <surf/surf.h>
#include <glm/glm.hpp>

#include <functional>

#include "Renderer.h"

namespace wv
{
	class ToolPanel
	{
	public:
		explicit ToolPanel(surf_Gapi gapi);
		~ToolPanel() = default;

		/// @brief Draws ImGui tools to the SDL window
		void Draw();

		/// @brief String for the model currently selected in the tool panel
		static std::string s_SelectedModel;

		/// @brief String for the material currently selected in the tool panel
		static std::string s_SelectedMaterial;

	private:
		std::function<void(void)> m_ImGuiNewFrameFunction;
	};
}