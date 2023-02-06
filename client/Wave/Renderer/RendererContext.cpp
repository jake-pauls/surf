#include "RendererContext.h"

#include "Vulkan/VkRendererContext.h"

wv::Unique<wv::RendererContext> wv::RendererContext::CreateRendererContext(wv::Window* window, wv::Renderer::GraphicsAPI api)
{
	switch (api)
	{
	case Renderer::GraphicsAPI::None:
		WAVE_ASSERT(false, "No rendering API selected");
		return nullptr;
	case Renderer::GraphicsAPI::DirectX:
		WAVE_ASSERT(false, "DirectX is unimplemented");
		return nullptr;
	case Renderer::GraphicsAPI::Vulkan:
		return CreateUnique<vkn::VkRendererContext>(window);
	}

	WAVE_ASSERT(false, "Unknown rendering API selected");
	return nullptr;
}
