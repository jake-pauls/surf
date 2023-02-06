#include "VkRenderer.h"

#include "VkHardware.h"
#include "VkRendererContext.h"
#include "VkShaderPipeline.h"

vkn::VkRenderer::VkRenderer(wv::Window* window)
	: m_Window(window)
	, m_VkHardware(window)
	, m_VkSwapChain(window, m_VkHardware)
{
}

void vkn::VkRenderer::Init()
{
	core::Log(ELogType::Trace, "[VkRenderer] Initializing Vulkan renderer");

	std::string triangleVertexShader = (core::FileSystem::GetSPIRVDirectory() / "Triangle.vert.spv").string();
	std::string triangleFragmentShader = (core::FileSystem::GetSPIRVDirectory() / "Triangle.frag.spv").string();
	VkShaderPipeline triangleShaderPipeline = VkShaderPipeline(m_VkHardware, triangleVertexShader, triangleFragmentShader);
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
