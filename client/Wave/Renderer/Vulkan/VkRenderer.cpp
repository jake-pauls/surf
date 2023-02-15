#include "VkRenderer.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include "VkPass.h"
#include "VkHardware.h"
#include "VkRendererContext.h"
#include "VkShaderPipeline.h"
#include "VkInitializers.h"

vkn::VkRenderer::VkRenderer(wv::Window* window)
	: m_Window(window)
	, m_TriangleMesh()
{
}

void vkn::VkRenderer::Init()
{
	core::Log(ELogType::Trace, "[VkRenderer] Initializing Vulkan renderer");

	// Default hardware
	{
		m_VkHardware = new VkHardware(m_Window);
		m_VkSwapChain = new VkSwapChain(*this, *m_VkHardware, m_Window);
		m_DefaultPass = new VkPass(m_VkHardware->m_LogicalDevice, *m_VkSwapChain);
	}

	// Shader generation
	{
		std::string triangleVertexShader = (core::FileSystem::GetShaderDirectory() / "TriangleMesh.vert.glsl.spv").string();
		std::string triangleFragmentShader = (core::FileSystem::GetShaderDirectory() / "TriangleMesh.frag.glsl.spv").string();
		m_TrianglePipeline = new VkShaderPipeline(*this, m_VkHardware->m_LogicalDevice, triangleVertexShader, triangleFragmentShader);
	}

	// Framebuffers (need to be initialized after initial renderpass)
	m_VkSwapChain->CreateFramebuffers();

	// Memory allocation
	{
		VmaAllocatorCreateInfo allocatorCreateInfo = VmaAllocatorCreateInfo();
		allocatorCreateInfo.physicalDevice = m_VkHardware->m_PhysicalDevice;
		allocatorCreateInfo.device = m_VkHardware->m_LogicalDevice;
		allocatorCreateInfo.instance = m_VkHardware->m_Instance;
		VK_CALL(vmaCreateAllocator(&allocatorCreateInfo, &m_Allocator));
	}

	// Synchronization objects
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

	LoadMeshes();
}

void vkn::VkRenderer::Draw()
{
	const std::vector<VkCommandBuffer>& commandBuffers = m_VkHardware->m_CommandBuffers;

	VK_CALL(vkWaitForFences(m_VkHardware->m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX));
	
	uint32_t imageIndex;
	VkResult acquireImageResult = vkAcquireNextImageKHR(m_VkHardware->m_LogicalDevice, m_VkSwapChain->m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);
	WAVE_ASSERT(acquireImageResult == VK_SUCCESS 
		|| acquireImageResult == VK_SUBOPTIMAL_KHR
		|| acquireImageResult == VK_ERROR_OUT_OF_DATE_KHR, "Failed to acquire next image from the swap chain");

	if (acquireImageResult == VK_ERROR_OUT_OF_DATE_KHR)
	{
		m_VkSwapChain->RecreateSwapchain();
		return;
	}

	// Only reset the fence if work is being submitted
	VK_CALL(vkResetFences(m_VkHardware->m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame]));

	VK_CALL(vkResetCommandBuffer(commandBuffers[m_CurrentFrame], 0));
	RecordCommandBuffer(commandBuffers[m_CurrentFrame], imageIndex);

	VkSubmitInfo submitInfo = VkSubmitInfo();
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame]};
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[m_CurrentFrame];

	VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VK_CALL(vkQueueSubmit(m_VkHardware->m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]));

	VkPresentInfoKHR presentationInfo = VkPresentInfoKHR();
	presentationInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentationInfo.waitSemaphoreCount = 1;
	presentationInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_VkSwapChain->m_SwapChain };
	presentationInfo.swapchainCount = 1;
	presentationInfo.pSwapchains = swapChains;
	presentationInfo.pImageIndices = &imageIndex;
	presentationInfo.pResults = nullptr;

	VkResult queuePresentResult = vkQueuePresentKHR(m_VkHardware->m_PresentationQueue, &presentationInfo);
	WAVE_ASSERT(queuePresentResult == VK_SUCCESS 
		|| queuePresentResult == VK_SUBOPTIMAL_KHR
		|| queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR, "Failed to present swap chain image");

	if (queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR 
		|| queuePresentResult == VK_SUBOPTIMAL_KHR 
		|| m_FramebufferResized)
	{
		m_FramebufferResized = false;
		m_VkSwapChain->RecreateSwapchain();
	}

	m_CurrentFrame = (m_CurrentFrame + 1) % c_MaxFramesInFlight;
}

void vkn::VkRenderer::Teardown()
{
	core::Log(ELogType::Trace, "[VkRenderer] Tearing down Vulkan renderer");

	// Wait for logical device to finish operations before tearing down
	VK_CALL(vkDeviceWaitIdle(m_VkHardware->m_LogicalDevice));

	// TODO: Abstract mesh services into VkMeshLoader?
	vmaDestroyBuffer(m_Allocator, m_TriangleMesh.m_VertexBuffer.m_Buffer, m_TriangleMesh.m_VertexBuffer.m_Allocation);
	vmaDestroyAllocator(m_Allocator);

	// TODO: Implement main destruction queue as opposed to relying on destructors?
	delete m_VkSwapChain;
	delete m_TrianglePipeline;
	delete m_DefaultPass;

	for (size_t i = 0; i < c_MaxFramesInFlight; ++i)
	{
		vkDestroySemaphore(m_VkHardware->m_LogicalDevice, m_ImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(m_VkHardware->m_LogicalDevice, m_RenderFinishedSemaphores[i], nullptr);
		vkDestroyFence(m_VkHardware->m_LogicalDevice, m_InFlightFences[i], nullptr);
	}

	// Teardown hardware (devices/instance) last
	delete m_VkHardware;
}

void vkn::VkRenderer::Clear() const
{
	// TODO
}

void vkn::VkRenderer::ClearColor() const
{
	// TODO
}

void vkn::VkRenderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	const VkExtent2D& swapChainExtent = m_VkSwapChain->m_SwapChainExtent;

	VkCommandBufferBeginInfo commandBufferInfo = vkn::InitCommandBufferBeginInfo();
	VK_CALL(vkBeginCommandBuffer(commandBuffer, &commandBufferInfo));

	VkRenderPassBeginInfo renderPassInfo = VkRenderPassBeginInfo();
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_DefaultPass->m_RenderPass;
	renderPassInfo.framebuffer = m_VkSwapChain->m_VkSwapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_VkSwapChain->m_SwapChainExtent;

	// Set clear color
	VkClearValue clearColor = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	// Start render pass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Bind required pipeline
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_TrianglePipeline->m_GraphicsPipeline);

	// Setup viewport/scissor
	VkViewport viewport = VkViewport();
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor = VkRect2D();
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	// Bind geometry
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_TriangleMesh.m_VertexBuffer.m_Buffer, &offset);

	// Draw geometry
	vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_TriangleMesh.m_Vertices.size()), 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);
	VK_CALL(vkEndCommandBuffer(commandBuffer));
}

void vkn::VkRenderer::LoadMeshes()
{
	m_TriangleMesh.m_Vertices.resize(3);

	// Vertex positions
	m_TriangleMesh.m_Vertices[0].m_Position = {  1.0f,  1.0f, 0.0f };
	m_TriangleMesh.m_Vertices[1].m_Position = { -1.0f,  1.0f, 0.0f };
	m_TriangleMesh.m_Vertices[2].m_Position = {  0.0f, -1.0f, 0.0f };

	// Vertex colors
	m_TriangleMesh.m_Vertices[0].m_Color = { 0.0f, 1.0f, 0.0f };
	m_TriangleMesh.m_Vertices[1].m_Color = { 0.0f, 1.0f, 0.0f };
	m_TriangleMesh.m_Vertices[2].m_Color = { 0.0f, 1.0f, 0.0f };

	UploadMesh(m_TriangleMesh);
}

void vkn::VkRenderer::UploadMesh(VkMesh& mesh)
{
	VkBufferCreateInfo bufferCreateInfo = VkBufferCreateInfo();
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = mesh.m_Vertices.size() * sizeof(VkVertex);
	bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	// Data should be writeable by CPU and readable by GPU
	VmaAllocationCreateInfo vmaAllocationCreateInfo = VmaAllocationCreateInfo();
	vmaAllocationCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
	VK_CALL(vmaCreateBuffer(m_Allocator, 
		&bufferCreateInfo, 
		&vmaAllocationCreateInfo, 
		&mesh.m_VertexBuffer.m_Buffer, 
		&mesh.m_VertexBuffer.m_Allocation, 
		nullptr));

	void* meshData;
	vmaMapMemory(m_Allocator, mesh.m_VertexBuffer.m_Allocation, &meshData);
	memcpy(meshData, mesh.m_Vertices.data(), mesh.m_Vertices.size() * sizeof(VkVertex));
	vmaUnmapMemory(m_Allocator, mesh.m_VertexBuffer.m_Allocation);
}
