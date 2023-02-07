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

	m_PassthroughPass = new VkPass(*this);

	std::string triangleVertexShader = (core::FileSystem::GetShaderDirectory() / "Triangle.vert.spv").string();
	std::string triangleFragmentShader = (core::FileSystem::GetShaderDirectory() / "Triangle.frag.spv").string();
	m_PassthroughPipeline = new VkShaderPipeline(*this, triangleVertexShader, triangleFragmentShader);

	core::Log(ELogType::Trace, "[VkRenderer] Successfully created first graphics pipeline");
}

void vkn::VkRenderer::Teardown()
{
	core::Log(ELogType::Trace, "[VkRenderer] Tearing down Vulkan renderer");

	delete m_PassthroughPipeline;
	delete m_PassthroughPass;
}

void vkn::VkRenderer::Clear() const
{
}

void vkn::VkRenderer::ClearColor() const
{
}
