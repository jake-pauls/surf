#include "VkRendererContext.h"

vk::VkRendererContext::VkRendererContext(wv::Window* window)
	: m_Window(window)
{
	WAVE_ASSERT(window, "Context window is being set to nullptr");
}

void vk::VkRendererContext::Init()
{
	core::Log(ELogType::Info, "[VkRendererContext] Initialized context for Vulkan renderer");
}

SDL_WindowFlags vk::VkRendererContext::GetContextSDLWindowFlags()
{
	return static_cast<SDL_WindowFlags>(SDL_WINDOW_VULKAN);
}
