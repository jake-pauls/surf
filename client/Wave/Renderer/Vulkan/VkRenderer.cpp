#include "VkRenderer.h"

#include <SDL3/SDL_vulkan.h>

#include <map>
#include <set>
#include <vector>

#include "VkRendererContext.h"

vkn::VkRenderer::VkRenderer(wv::Window* window)
	: m_Window(window)
{
	m_VkHardware = new VkHardware(window);
}

vkn::VkRenderer::~VkRenderer()
{
	delete m_VkHardware;
}

void vkn::VkRenderer::Init()
{
	m_VkHardware->Init();
}

void vkn::VkRenderer::Teardown() const
{
	core::Log(ELogType::Trace, "[VkRenderer] Tearing down Vulkan renderer");

	m_VkHardware->Teardown();
}

void vkn::VkRenderer::Clear() const
{
}

void vkn::VkRenderer::ClearColor() const
{
}
