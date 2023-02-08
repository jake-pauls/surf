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

	// Pipeline
	{
		m_PassthroughPass = new VkPass(*this);

		std::string triangleVertexShader = (core::FileSystem::GetShaderDirectory() / "Triangle.vert.spv").string();
		std::string triangleFragmentShader = (core::FileSystem::GetShaderDirectory() / "Triangle.frag.spv").string();
		m_PassthroughPipeline = new VkShaderPipeline(*this, triangleVertexShader, triangleFragmentShader);

		core::Log(ELogType::Trace, "[VkRenderer] Successfully created first graphics pipeline");
	}

	// Framebuffer
	{
		const VkExtent2D& swapChainExtent = m_VkSwapChain.m_SwapChainExtent;
		const std::vector<VkImageView>& swapChainImageViews = m_VkSwapChain.m_SwapChainImageViews;

		m_VkSwapChainFramebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); ++i)
		{
			VkImageView attachments[] = { swapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferCreateInfo = VkFramebufferCreateInfo();
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = m_PassthroughPass->m_RenderPass;
			framebufferCreateInfo.attachmentCount = 1;
			framebufferCreateInfo.pAttachments = attachments;
			framebufferCreateInfo.width = swapChainExtent.width;
			framebufferCreateInfo.height = swapChainExtent.height;
			framebufferCreateInfo.layers = 1;

			VK_CALL(vkCreateFramebuffer(m_VkHardware.m_LogicalDevice, &framebufferCreateInfo, nullptr, &m_VkSwapChainFramebuffers[i]));
		}
	}

	// Command buffer allocation
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = VkCommandBufferAllocateInfo();
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = m_VkHardware.m_CommandPool;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandBufferCount = 1;

		VK_CALL(vkAllocateCommandBuffers(m_VkHardware.m_LogicalDevice, &commandBufferAllocateInfo, &m_CommandBuffer));
	}

	// Synchronization objects
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo = VkSemaphoreCreateInfo();
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		
		VkFenceCreateInfo fenceCreateInfo = VkFenceCreateInfo();
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VK_CALL(vkCreateSemaphore(m_VkHardware.m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphore));
		VK_CALL(vkCreateSemaphore(m_VkHardware.m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_RenderFinishedSemaphore));
		VK_CALL(vkCreateFence(m_VkHardware.m_LogicalDevice, &fenceCreateInfo, nullptr, &m_InFlightFence));
	}
}

void vkn::VkRenderer::Draw()
{
	VK_CALL(vkWaitForFences(m_VkHardware.m_LogicalDevice, 1, &m_InFlightFence, VK_TRUE, UINT64_MAX));
	VK_CALL(vkResetFences(m_VkHardware.m_LogicalDevice, 1, &m_InFlightFence));
	
	uint32_t imageIndex;
	VK_CALL(vkAcquireNextImageKHR(m_VkHardware.m_LogicalDevice, m_VkSwapChain.m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

	VK_CALL(vkResetCommandBuffer(m_CommandBuffer, 0));
	RecordCommandBuffer(m_CommandBuffer, imageIndex);

	VkSubmitInfo submitInfo = VkSubmitInfo();
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffer;

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

	delete m_PassthroughPipeline;
	delete m_PassthroughPass;
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

	VkCommandBufferBeginInfo commandBufferInfo = VkCommandBufferBeginInfo();
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferInfo.flags = 0;
	commandBufferInfo.pInheritanceInfo = nullptr;

	VK_CALL(vkBeginCommandBuffer(commandBuffer, &commandBufferInfo));

	VkRenderPassBeginInfo renderPassInfo = VkRenderPassBeginInfo();
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_PassthroughPass->m_RenderPass;
	renderPassInfo.framebuffer = m_VkSwapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_VkSwapChain.m_SwapChainExtent;

	VkClearValue clearColor = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PassthroughPipeline->m_GraphicsPipeline);

	VkViewport viewport = VkViewport();
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

	VkRect2D scissor = VkRect2D();
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);

	vkCmdDraw(m_CommandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(m_CommandBuffer);
	VK_CALL(vkEndCommandBuffer(m_CommandBuffer));
}
