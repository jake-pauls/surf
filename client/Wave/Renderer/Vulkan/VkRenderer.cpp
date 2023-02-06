#include "VkRenderer.h"

#include <SDL3/SDL_vulkan.h>

#include <map>
#include <set>
#include <vector>

#include "VkHardware.h"
#include "VkRendererContext.h"

vkn::VkRenderer::VkRenderer(wv::Window* window)
	: m_Window(window)
	, m_VkHardware(window)
	, m_VkSwapChain(window, m_VkHardware)
{
}

void vkn::VkRenderer::Init()
{
	core::Log(ELogType::Trace, "[VkRenderer] Initializing Vulkan renderer");
}

void vkn::VkRenderer::Teardown()
{
	core::Log(ELogType::Trace, "[VkRenderer] Tearing down Vulkan renderer");
}

void vkn::VkRenderer::Clear() const
{
}

void vkn::VkRenderer::ClearColor() const
{
}
