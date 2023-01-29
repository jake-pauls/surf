#include "Renderer.h"

#include "Vulkan/VkRenderer.h"

wv::Unique<wv::Renderer> wv::Renderer::CreateRendererWithAPISpec(wv::Renderer::GraphicsAPI api)
{
	switch (api)
	{
	case wv::Renderer::GraphicsAPI::None:
		WAVE_ASSERT(false, "No rendering API selected");
		return nullptr;
	case wv::Renderer::GraphicsAPI::Vulkan:
		return wv::CreateUnique<vk::VkRenderer>();
	};

	WAVE_ASSERT(false, "Unknown rendering API selected");
	return nullptr;
}

