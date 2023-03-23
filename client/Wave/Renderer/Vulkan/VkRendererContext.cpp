#include "VkRendererContext.h"

#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

vkn::VkRendererContext::VkRendererContext(wv::Window* window)
	: m_Window(window)
{
	WAVE_ASSERT(window, "Context window is being set to nullptr");
}

void vkn::VkRendererContext::Init()
{
	core::Log(ELogType::Info, "[VkRendererContext] Initialized context for Vulkan renderer");
}

SDL_WindowFlags vkn::VkRendererContext::GetContextSDLWindowFlags() const
{
	return static_cast<SDL_WindowFlags>(SDL_WINDOW_VULKAN);
}

std::vector<const char*> vkn::VkRendererContext::GetVulkanContextExtensions(bool vLayerExtensions /* = false */) const
{
	SDL_Window* window = m_Window->GetSDLWindow();

	// Retrieve required SDL extensions
	uint32_t contextExtensionCount = 0;
	SDL_Vulkan_GetInstanceExtensions(&contextExtensionCount, nullptr);
	std::vector<const char*> contextExtensions = std::vector<const char*>(contextExtensionCount);
	SDL_Vulkan_GetInstanceExtensions(&contextExtensionCount, contextExtensions.data());

	// Ensure that the debug utils extension is included if validation layers are enabled
	if (vLayerExtensions)
	{
		contextExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return contextExtensions;
}

void vkn::VkRendererContext::SetupSDLVulkanSurface(VkInstance instance, VkSurfaceKHR* surface) const
{
	SDL_Vulkan_CreateSurface(m_Window->GetSDLWindow(), instance, surface);
}

VkExtent2D vkn::VkRendererContext::GetVulkanClientExtent() const
{
	int width, height;

	SDL_GetWindowSize(m_Window->GetSDLWindow(), &width, &height);

	return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}
