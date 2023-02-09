#include "VkRenderer.h"

#include "VkPass.h"
#include "VkHardware.h"
#include "VkRendererContext.h"
#include "VkShaderPipeline.h"
#include "VkInitializers.h"

vkn::VkRenderer::VkRenderer(wv::Window* window)
	: m_Window(window)
	, m_VkHardware(window)
	, m_VkSwapChain(window, m_VkHardware)
{
}

void vkn::VkRenderer::Init()
{
	core::Log(ELogType::Trace, "[VkRenderer] Initializing Vulkan renderer");

	// Pipeline
	m_DefaultPass = new VkPass(m_VkHardware.m_LogicalDevice, m_VkSwapChain);

	std::string triangleVertexShader = (core::FileSystem::GetShaderDirectory() / "Triangle.vert.hlsl.spv").string();
	std::string triangleFragmentShader = (core::FileSystem::GetShaderDirectory() / "Triangle.frag.hlsl.spv").string();
	m_DefaultPipeline = new VkShaderPipeline(
		m_VkHardware.m_LogicalDevice, 
		m_DefaultPass->m_RenderPass, 
		triangleVertexShader, 
		triangleFragmentShader
	);

	if (m_DefaultPass && m_DefaultPipeline)
		core::Log(ELogType::Trace, "[VkRenderer] Successfully created first graphics pipeline");

	// Framebuffer
	{
		const VkExtent2D& swapChainExtent = m_VkSwapChain.m_SwapChainExtent;
		const std::vector<VkImageView>& swapChainImageViews = m_VkSwapChain.m_SwapChainImageViews;

		m_VkSwapChainFramebuffers.resize(swapChainImageViews.size());

		VkFramebufferCreateInfo framebufferCreateInfo = vkn::InitFramebufferCreateInfo(m_DefaultPass->m_RenderPass, swapChainExtent);
		for (size_t i = 0; i < swapChainImageViews.size(); ++i)
		{
			framebufferCreateInfo.pAttachments = &swapChainImageViews[i];
			VK_CALL(vkCreateFramebuffer(m_VkHardware.m_LogicalDevice, &framebufferCreateInfo, nullptr, &m_VkSwapChainFramebuffers[i]));
		}
	}

	// Synchronization objects
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo = vkn::InitSemaphoreCreateInfo();
		VkFenceCreateInfo fenceCreateInfo = vkn::InitFenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);

		VK_CALL(vkCreateSemaphore(m_VkHardware.m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphore));
		VK_CALL(vkCreateSemaphore(m_VkHardware.m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_RenderFinishedSemaphore));
		VK_CALL(vkCreateFence(m_VkHardware.m_LogicalDevice, &fenceCreateInfo, nullptr, &m_InFlightFence));
	}
}

void vkn::VkRenderer::Draw()
{
	const VkCommandBuffer& commandBuffer = m_VkHardware.m_CommandBuffer;

	VK_CALL(vkWaitForFences(m_VkHardware.m_LogicalDevice, 1, &m_InFlightFence, VK_TRUE, UINT64_MAX));
	VK_CALL(vkResetFences(m_VkHardware.m_LogicalDevice, 1, &m_InFlightFence));
	
	uint32_t imageIndex;
	VK_CALL(vkAcquireNextImageKHR(m_VkHardware.m_LogicalDevice, m_VkSwapChain.m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

	VK_CALL(vkResetCommandBuffer(commandBuffer, 0));
	RecordCommandBuffer(commandBuffer, imageIndex);

	VkSubmitInfo submitInfo = VkSubmitInfo();
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VK_CALL(vkQueueSubmit(m_VkHardware.m_GraphicsQueue, 1, &submitInfo, m_InFlightFence));

	VkPresentInfoKHR presentationInfo = VkPresentInfoKHR();
	presentationInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentationInfo.waitSemaphoreCount = 1;
	presentationInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_VkSwapChain.m_SwapChain };
	presentationInfo.swapchainCount = 1;
	presentationInfo.pSwapchains = swapChains;
	presentationInfo.pImageIndices = &imageIndex;
	presentationInfo.pResults = nullptr;

	VK_CALL(vkQueuePresentKHR(m_VkHardware.m_PresentationQueue, &presentationInfo));
}

void vkn::VkRenderer::Teardown()
{
	core::Log(ELogType::Trace, "[VkRenderer] Tearing down Vulkan renderer");

	// Wait for logical device to finish operations before tearing down
	VK_CALL(vkDeviceWaitIdle(m_VkHardware.m_LogicalDevice));

	vkDestroySemaphore(m_VkHardware.m_LogicalDevice, m_ImageAvailableSemaphore, nullptr);
	vkDestroySemaphore(m_VkHardware.m_LogicalDevice, m_RenderFinishedSemaphore, nullptr);
	vkDestroyFence(m_VkHardware.m_LogicalDevice, m_InFlightFence, nullptr);

	for (VkFramebuffer framebuffer : m_VkSwapChainFramebuffers)
		vkDestroyFramebuffer(m_VkHardware.m_LogicalDevice, framebuffer, nullptr);

	delete m_DefaultPass;
	delete m_DefaultPipeline;
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
	const VkExtent2D& swapChainExtent = m_VkSwapChain.m_SwapChainExtent;

	VkCommandBufferBeginInfo commandBufferInfo = vkn::InitCommandBufferBeginInfo();
	VK_CALL(vkBeginCommandBuffer(commandBuffer, &commandBufferInfo));

	VkRenderPassBeginInfo renderPassInfo = VkRenderPassBeginInfo();
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_DefaultPass->m_RenderPass;
	renderPassInfo.framebuffer = m_VkSwapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_VkSwapChain.m_SwapChainExtent;

	VkClearValue clearColor = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_DefaultPipeline->m_GraphicsPipeline);

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

	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);
	VK_CALL(vkEndCommandBuffer(commandBuffer));
}
