#include "RendererContext.h"

#include "Vulkan/VkRendererContext.h"

core::Unique<wv::RendererContext> wv::RendererContext::CreateRendererContext(wv::Window* window, surf_Gapi gapi)
{
	switch (gapi)
	{
	case SURF_GAPI_NIL:
		WAVE_ASSERT(false, "No rendering API selected");
		return nullptr;
	case SURF_GAPI_DIRECTX:
		WAVE_ASSERT(false, "DirectX is unimplemented");
		return nullptr;
	case SURF_GAPI_OPENGL:
		WAVE_ASSERT(false, "OpenGL is unimplemented");
	case SURF_GAPI_VULKAN:
		return core::CreateUnique<vkn::VkRendererContext>(window);
	}

	WAVE_ASSERT(false, "Unknown rendering API selected");
	return nullptr;
}
