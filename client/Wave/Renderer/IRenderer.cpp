#include "IRenderer.h"

#include "Vulkan/VkRenderer.h"

wv::Unique<wv::IRenderer> wv::IRenderer::CreateRendererWithGAPI(Window* window, IRenderer::GraphicsAPI gapi)
{
	switch (gapi)
	{
	case IRenderer::GraphicsAPI::None:
		WAVE_ASSERT(false, "No rendering API selected");
		return nullptr;
	case IRenderer::GraphicsAPI::DirectX:
		WAVE_ASSERT(false, "DirectX is unimplemented");
		return nullptr;
	case IRenderer::GraphicsAPI::Vulkan:
		return CreateUnique<wvk::VkRenderer>(window);
	};

	WAVE_ASSERT(false, "Unknown rendering API selected");
	return nullptr;
}

