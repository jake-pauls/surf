#include "IRendererContext.h"

#include "Vulkan/VkRendererContext.h"

wv::Unique<wv::IRendererContext> wv::IRendererContext::CreateRendererContext(wv::Window* window, wv::IRenderer::GraphicsAPI api)
{
	switch (api)
	{
	case IRenderer::GraphicsAPI::None:
		WAVE_ASSERT(false, "No rendering API selected");
		return nullptr;
	case IRenderer::GraphicsAPI::DirectX:
		WAVE_ASSERT(false, "DirectX is unimplemented");
		return nullptr;
	case IRenderer::GraphicsAPI::Vulkan:
		return CreateUnique<vkn::VkRendererContext>(window);
	}

	WAVE_ASSERT(false, "Unknown rendering API selected");
	return nullptr;
}
