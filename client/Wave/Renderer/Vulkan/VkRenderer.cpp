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
	// Initialize hardware harness first
	m_VkHardware.Init();

	// Create swap chains
	m_VkSwapChain.Create();
}

void vkn::VkRenderer::Teardown()
{
	core::Log(ELogType::Trace, "[VkRenderer] Tearing down Vulkan renderer");

	// Destroy existing swap chains first
	m_VkSwapChain.Destroy();

	// Teardown hardware last
	m_VkHardware.Teardown();
}

void vkn::VkRenderer::Clear() const
{
}

void vkn::VkRenderer::ClearColor() const
{
}
