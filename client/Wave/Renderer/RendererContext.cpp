#include "RendererContext.h"

#include "Vulkan/VkRendererContext.h"

wv::Unique<wv::RendererContext> wv::RendererContext::CreateRendererContext(wv::Window* window, wv::Renderer::GraphicsAPI api)
{
	switch (api)
	{
	case wv::Renderer::GraphicsAPI::None:
		WAVE_ASSERT(false, "No rendering API selected");
		return nullptr;
	case wv::Renderer::GraphicsAPI::Vulkan:
		return wv::CreateUnique<vk::VkRendererContext>(window);
	}

	WAVE_ASSERT(false, "Unknown rendering API selected");
	return nullptr;
}
