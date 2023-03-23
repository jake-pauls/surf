#include "Renderer.h"

#include "Vulkan/VkRenderer.h"

core::Unique<wv::Renderer> wv::Renderer::CreateRendererWithSurfCfg(Window* window, Camera* camera, surf_Cfg cfg)
{
	switch (cfg.Gapi)
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
		return core::CreateUnique<vkn::VkRenderer>(window, camera);
	};

	WAVE_ASSERT(false, "Unknown rendering API selected");
	return nullptr;
}

