#include "VkRenderer.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include <glm/gtx/transform.hpp>

#include "VkPass.h"
#include "VkModel.h"
#include "VkHardware.h"
#include "VkRendererContext.h"
#include "VkShaderPipeline.h"
#include "VkInitializers.h"

vkn::VkRenderer::VkRenderer(wv::Window* window)
	: m_Window(window)
{
}

void vkn::VkRenderer::Init()
{
	core::Log(ELogType::Trace, "[VkRenderer] Initializing Vulkan renderer");

	// Default hardware
	{
		m_VkHardware = new VkHardware(m_Window);
	}

	// Swap chain and render passes
	{
		m_VkSwapChain = new VkSwapChain(*this, *m_VkHardware, m_Window);
		m_DefaultPass = new VkPass(m_VkHardware->m_LogicalDevice, *m_VkSwapChain);

		// Framebuffers (need to be initialized after initial renderpass)
		m_VkSwapChain->CreateFramebuffers();
	}

	// Shader generation
	{
		std::string triangleVertexShader = core::FileSystem::GetShaderPath("TriangleMesh.vert.glsl.spv").string();
		std::string triangleFragmentShader = core::FileSystem::GetShaderPath("TriangleMesh.frag.glsl.spv").string();
		m_DefaultPipeline = new VkShaderPipeline(*this, m_VkHardware->m_LogicalDevice, triangleVertexShader, triangleFragmentShader);
	}

	// Sync objects
	{
		m_ImageAvailableSemaphores.resize(c_MaxFramesInFlight);
		m_RenderFinishedSemaphores.resize(c_MaxFramesInFlight);
		m_InFlightFences.resize(c_MaxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreCreateInfo = vkn::InitSemaphoreCreateInfo();
		VkFenceCreateInfo fenceCreateInfo = vkn::InitFenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);

		for (size_t i = 0; i < c_MaxFramesInFlight; ++i)
		{
			VK_CALL(vkCreateSemaphore(m_VkHardware->m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphores[i]));
			VK_CALL(vkCreateSemaphore(m_VkHardware->m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_RenderFinishedSemaphores[i]));
			VK_CALL(vkCreateFence(m_VkHardware->m_LogicalDevice, &fenceCreateInfo, nullptr, &m_InFlightFences[i]));
		}
	}

	// Command buffer allocations
	CreateCommandBuffers();

	// Load 3D meshes
	LoadMeshes();
}

void vkn::VkRenderer::Draw()
{
	VkCommandBuffer commandBuffer = BeginFrame();
	if (commandBuffer == VK_NULL_HANDLE) 
		return;

	BeginRenderPass(commandBuffer);

	DrawCommandBuffer(commandBuffer);

	EndRenderPass(commandBuffer);

	EndFrame();
}

void vkn::VkRenderer::Teardown()
{
	core::Log(ELogType::Trace, "[VkRenderer] Tearing down Vulkan renderer");

	// Wait for logical device to finish operations before tearing down
	VK_CALL(vkDeviceWaitIdle(m_VkHardware->m_LogicalDevice));

	delete m_LoadedModel;
	//delete m_TriangleModel

	// TODO: Implement main destruction queue as opposed to relying on destructors?
	delete m_VkSwapChain;
	delete m_DefaultPipeline;
	delete m_DefaultPass;

	for (size_t i = 0; i < c_MaxFramesInFlight; ++i)
	{
		vkDestroySemaphore(m_VkHardware->m_LogicalDevice, m_ImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(m_VkHardware->m_LogicalDevice, m_RenderFinishedSemaphores[i], nullptr);
		vkDestroyFence(m_VkHardware->m_LogicalDevice, m_InFlightFences[i], nullptr);
	}

	// Free command buffers
	vkFreeCommandBuffers(m_VkHardware->m_LogicalDevice, m_VkHardware->m_CommandPool, static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
	m_CommandBuffers.clear();

	// Teardown hardware (devices/instance) last
	delete m_VkHardware;
}

void vkn::VkRenderer::CreateCommandBuffers()
{
	m_CommandBuffers.resize(c_MaxFramesInFlight);

	auto commandBufferAllocateInfo = vkn::InitCommandBufferAllocateInfo(m_VkHardware->m_CommandPool, 2);
	VK_CALL(vkAllocateCommandBuffers(m_VkHardware->m_LogicalDevice, &commandBufferAllocateInfo, m_CommandBuffers.data()));
}

VkCommandBuffer vkn::VkRenderer::BeginFrame()
{
	WAVE_ASSERT(!m_IsFrameStarted, "Cannot call BeginFrame while another frame is in progress");

	VkResult result = m_VkSwapChain->AcquireNextImage(&m_CurrentImageIndex);

	WAVE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR, "Failed to acquire next image from the swap chain");
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		m_VkSwapChain->RecreateSwapchain();
		return VK_NULL_HANDLE;
	}

	VK_CALL(vkResetFences(m_VkHardware->m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrameIndex]));

	m_IsFrameStarted = true;

	VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
	VK_CALL(vkResetCommandBuffer(commandBuffer, 0));

	VkCommandBufferBeginInfo commandBufferInfo = vkn::InitCommandBufferBeginInfo();
	VK_CALL(vkBeginCommandBuffer(commandBuffer, &commandBufferInfo));

	return commandBuffer;
}

void vkn::VkRenderer::EndFrame()
{
	WAVE_ASSERT(m_IsFrameStarted, "Cannot call BeginFrame without a frame in progress");

	VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
	VK_CALL(vkEndCommandBuffer(commandBuffer));

	VkResult result = m_VkSwapChain->SubmitCommandBuffers(&commandBuffer, m_CurrentImageIndex);
	WAVE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR, "Failed to present swap chain image");

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		m_VkSwapChain->RecreateSwapchain();

	m_IsFrameStarted = false;
	m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % c_MaxFramesInFlight;
}

void vkn::VkRenderer::BeginRenderPass(VkCommandBuffer commandBuffer)
{
	WAVE_ASSERT(m_IsFrameStarted, "Cannot begin render pass without a frame in progress");
	WAVE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Cannot end render pass on a command buffer from a different frame");

	const VkExtent2D& swapChainExtent = m_VkSwapChain->m_SwapChainExtent;
	auto renderPassInfo = vkn::InitRenderPassBeginInfo(m_DefaultPass->m_RenderPass, m_VkSwapChain->m_VkSwapChainFramebuffers[m_CurrentImageIndex], swapChainExtent);

	VkClearValue clearColor;
	clearColor.color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
	VkClearValue depthClear;
	depthClear.depthStencil.depth = 1.0f;

	VkClearValue clearValues[] = { clearColor, depthClear };
	renderPassInfo.pClearValues = &clearValues[0];
	renderPassInfo.clearValueCount = 2;

	// Start render pass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Setup viewport/scissor
	VkViewport viewport = VkViewport();
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = VkRect2D();
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void vkn::VkRenderer::DrawCommandBuffer(VkCommandBuffer commandBuffer)
{
	// Bind required pipeline
	m_DefaultPipeline->Bind(commandBuffer);

	// Bind geometry
	m_LoadedModel->Bind(commandBuffer);
	// m_TriangleModel->Bind(commandBuffer);

	// MVP/Uniform Testing
	glm::vec3 cameraPosition = { 1.0f, -1.0f, -8.0f };
	glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), cameraPosition);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 200.0f);
	projectionMatrix[1][1] *= -1;

	glm::mat4 modelMatrix = glm::translate(glm::mat4(0.0f), glm::vec3(0.0f));
	modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(core::Timer::DeltaTime() * 0.02f), glm::vec3(0, 1, 0));

	glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

	// Submit push constants (uniforms)
	VkMeshPushConstants constants;
	constants.m_MvpMatrix = mvpMatrix;
	vkCmdPushConstants(commandBuffer, m_DefaultPipeline->m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(VkMeshPushConstants), &constants);

	// Draw geometry
	m_LoadedModel->Draw(commandBuffer);
	// m_TriangleModel->Draw(commandBuffer);
}

void vkn::VkRenderer::EndRenderPass(VkCommandBuffer commandBuffer)
{
	WAVE_ASSERT(m_IsFrameStarted, "Cannot end render pass without a frame in progress");
	WAVE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Cannot render pass on a command buffer from a different frame");

	vkCmdEndRenderPass(commandBuffer);
}

void vkn::VkRenderer::LoadMeshes()
{
	m_TriangleMesh.m_Vertices.resize(4);

	// Vertex positions
	m_TriangleMesh.m_Vertices[0].m_Position = {  1.0f,  1.0f, 0.0f };
	m_TriangleMesh.m_Vertices[1].m_Position = {  1.0f, -1.0f, 0.0f };
	m_TriangleMesh.m_Vertices[2].m_Position = { -1.0f, -1.0f, 0.0f };
	m_TriangleMesh.m_Vertices[3].m_Position = { -1.0f,  1.0f, 0.0f };

	// Vertex colors
	m_TriangleMesh.m_Vertices[0].m_Color = { 1.0f, 0.0f, 0.0f };
	m_TriangleMesh.m_Vertices[1].m_Color = { 0.0f, 1.0f, 0.0f };
	m_TriangleMesh.m_Vertices[2].m_Color = { 0.0f, 0.0f, 1.0f };
	m_TriangleMesh.m_Vertices[3].m_Color = { 1.0f, 0.0f, 0.0f };

	// Indices
	m_TriangleMesh.m_Indices = { 0, 1, 3, 1, 2, 3 };
	//m_TriangleModel = new VkModel(*m_VkHardware, m_TriangleMesh);

	m_LoadedMesh.LoadFromObj(core::FileSystem::GetAssetPath("suzanne.obj").string().c_str());
	m_LoadedModel = new VkModel(*m_VkHardware, m_LoadedMesh);
}

