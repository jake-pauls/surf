#include "ToolPanel.h"

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_sdl3.h>

namespace wv
{
	std::string ToolPanel::s_SelectedModel = "Viking Room";
}

wv::ToolPanel::ToolPanel(Renderer::GraphicsAPI gapi)
{
	switch (gapi)
	{
	case Renderer::GraphicsAPI::None:
		WAVE_ASSERT(false, "No graphics API selected");
		break;
	case Renderer::GraphicsAPI::DirectX:
		WAVE_ASSERT(false, "DirectX is unimplemented");
		break;
	case Renderer::GraphicsAPI::Vulkan:
		m_ImGuiNewFrameFunction = &ImGui_ImplVulkan_NewFrame;
		break;
	}
}

void wv::ToolPanel::Draw()
{
	m_ImGuiNewFrameFunction();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	// Note: 'VkRenderer' matches against these strings when selecting new meshes
	const char* models[] = { "Viking Room", "Teapot", "Bunny", "Suzanne", "Dragon" };
	static const char* currentItem = models[0];
	ImGui::Begin("wave. tool panel");
	ImGui::SetWindowSize(ImVec2(350.0f, 70.0f), ImGuiCond_Always);
	ImGui::SetWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always);
	if (ImGui::BeginCombo("Selected Model", currentItem))
	{
		for (size_t i = 0; i < IM_ARRAYSIZE(models); i++)
		{
			bool isSelected = currentItem == models[i];
			if (ImGui::Selectable(models[i], isSelected))
			{
				currentItem = models[i];
				s_SelectedModel = currentItem;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::End();
}
