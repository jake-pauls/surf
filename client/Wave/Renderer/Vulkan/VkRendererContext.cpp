#include "VkRendererContext.h"

#include <SDL3/SDL_vulkan.h>

wvk::VkRendererContext::VkRendererContext(wv::Window* window)
	: m_Window(window)
{
	WAVE_ASSERT(window, "Context window is being set to nullptr");
}

void wvk::VkRendererContext::Init()
{
	core::Log(ELogType::Info, "[VkRendererContext] Initialized context for Vulkan renderer");
}

SDL_WindowFlags wvk::VkRendererContext::GetContextSDLWindowFlags()
{
	return static_cast<SDL_WindowFlags>(SDL_WINDOW_VULKAN);
}

std::vector<const char*> wvk::VkRendererContext::GetVulkanContextExtensions(bool vLayerExtensions /* = false */) const
{
	SDL_Window* window = m_Window->GetSDLWindow();

	// Retrieve required SDL extensions
	uint32_t contextExtensionCount = 0;
	SDL_Vulkan_GetInstanceExtensions(window, &contextExtensionCount, nullptr);
	std::vector<const char*> contextExtensions = std::vector<const char*>(contextExtensionCount);
	SDL_Vulkan_GetInstanceExtensions(window, &contextExtensionCount, contextExtensions.data());

	// Ensure that the debug utils extension is included if validation layers are enabled
	if (vLayerExtensions)
		contextExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return contextExtensions;
}
