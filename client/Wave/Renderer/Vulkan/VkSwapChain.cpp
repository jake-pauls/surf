#include "VkSwapChain.h"

#include <cmath>
#include <algorithm>

#include "Window.h"
#include "VkRenderer.h"
#include "VkHardware.h"
#include "VkRendererContext.h"
#include "VkInitializers.h"

vkn::VkSwapChain::VkSwapChain(
	const VkRenderer& renderer,
	const VkHardware& hardware,
	wv::Window* window
)
	: m_Window(window)
	, c_VkRenderer(renderer)
	, c_VkHardware(hardware)
{
	core::Log(ELogType::Trace, "[VkSwapChain] Creating a swap chain");

	CreateSwapchain();
	CreateImageViews();
	CreateDepthImage();
}

vkn::VkSwapChain::~VkSwapChain()
{
	core::Log(ELogType::Trace, "[VkSwapChain] Destroying swap chain");

	Destroy();
}

void vkn::VkSwapChain::RecreateSwapchain()
{
	VK_CALL(vkDeviceWaitIdle(c_VkHardware.m_LogicalDevice));

	// Cleanup swap chain before recreation
	Destroy();

	CreateSwapchain();
	CreateImageViews();
	CreateDepthImage();
	CreateFramebuffers();
}

void vkn::VkSwapChain::CreateSwapchain() 
{
	// Validate swap chain with corresponding physical device before starting
	SwapChainSupportDetails swapChainSupportDetails = QuerySwapChainSupport(c_VkHardware.m_PhysicalDevice);
	WAVE_ASSERT(!swapChainSupportDetails.m_SurfaceFormats.empty() && !swapChainSupportDetails.m_PresentationModes.empty(), "Swap chain is not properly supported by this physical device");

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupportDetails.m_SurfaceFormats);
	VkPresentModeKHR presentationMode = ChooseSwapPresentationMode(swapChainSupportDetails.m_PresentationModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupportDetails.m_SurfaceCapabilities);

	// Recommendation: Always have an extra image in the event that the swap chain is waiting on the driver
	uint32_t imageCount = swapChainSupportDetails.m_SurfaceCapabilities.minImageCount + 1;
	if (swapChainSupportDetails.m_SurfaceCapabilities.maxImageCount > 0 &&
		imageCount > swapChainSupportDetails.m_SurfaceCapabilities.maxImageCount)
		imageCount = swapChainSupportDetails.m_SurfaceCapabilities.maxImageCount;

	VkSwapchainCreateInfoKHR swapChainCreateInfo = VkSwapchainCreateInfoKHR();
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = c_VkHardware.m_Surface;
	swapChainCreateInfo.minImageCount = imageCount;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = extent;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamily queueFamily = c_VkHardware.FindQueueFamilies(c_VkHardware.m_PhysicalDevice);
	uint32_t queueFamilies[] = { queueFamily.m_GraphicsFamily.value(), queueFamily.m_PresentationFamily.value() };
	if (queueFamily.m_GraphicsFamily != queueFamily.m_PresentationFamily)
	{
		// Ownership must be transferred between queue families
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilies;
	}
	else
	{
		// Images can be used across queue families without transfers
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
	}

	swapChainCreateInfo.preTransform = swapChainSupportDetails.m_SurfaceCapabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = presentationMode;
	swapChainCreateInfo.clipped = VK_TRUE;

	// TODO: Pass reference to old swap chain here when create a new one (ie: when framebuffer is resized)
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_CALL(vkCreateSwapchainKHR(c_VkHardware.m_LogicalDevice, &swapChainCreateInfo, nullptr, &m_SwapChain));

	// Retrieve initial swap chain images
	VK_CALL(vkGetSwapchainImagesKHR(c_VkHardware.m_LogicalDevice, m_SwapChain, &imageCount, nullptr));
	m_SwapChainImages.resize(imageCount);
	VK_CALL(vkGetSwapchainImagesKHR(c_VkHardware.m_LogicalDevice, m_SwapChain, &imageCount, m_SwapChainImages.data()));

	// Save current swap chain format and extent
	m_SwapChainImageFormat = surfaceFormat.format;
	m_SwapChainExtent = extent;
}

void vkn::VkSwapChain::CreateImageViews()
{
	// Create image views
	m_SwapChainImageViews.resize(m_SwapChainImages.size());
	for (size_t i = 0; i < m_SwapChainImages.size(); ++i)
	{
		auto imageViewCreateInfo = vkn::InitImageViewCreateInfo(m_SwapChainImageFormat, m_SwapChainImages[i], VK_IMAGE_ASPECT_COLOR_BIT);

		VK_CALL(vkCreateImageView(c_VkHardware.m_LogicalDevice, &imageViewCreateInfo, nullptr, &m_SwapChainImageViews[i]));
	}
}

void vkn::VkSwapChain::CreateDepthImage()
{
	VkExtent3D depthImageExtent = { m_SwapChainExtent.width, m_SwapChainExtent.height, 1 };

	m_DepthFormat = VK_FORMAT_D32_SFLOAT;
	auto depthImageCreateInfo = vkn::InitImageCreateInfo(m_DepthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);

	VmaAllocationCreateInfo depthImageAllocationCreateInfo = VmaAllocationCreateInfo();
	depthImageAllocationCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	depthImageAllocationCreateInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VK_CALL(vmaCreateImage(c_VkRenderer.m_VmaAllocator, &depthImageCreateInfo, &depthImageAllocationCreateInfo, &m_DepthImage.m_Image, &m_DepthImage.m_Allocation, nullptr));

	auto depthImageViewCreateInfo = vkn::InitImageViewCreateInfo(m_DepthFormat, m_DepthImage.m_Image, VK_IMAGE_ASPECT_DEPTH_BIT);
	VK_CALL(vkCreateImageView(c_VkHardware.m_LogicalDevice, &depthImageViewCreateInfo, nullptr, &m_DepthImageView));
}

void vkn::VkSwapChain::CreateFramebuffers()
{
	const std::vector<VkImageView>& swapChainImageViews = m_SwapChainImageViews;

	m_VkSwapChainFramebuffers.resize(swapChainImageViews.size());

	auto framebufferCreateInfo = vkn::InitFramebufferCreateInfo(c_VkRenderer.m_DefaultPass->m_RenderPass, m_SwapChainExtent);
	for (size_t i = 0; i < swapChainImageViews.size(); ++i)
	{
		VkImageView attachments[2];
		attachments[0] = m_SwapChainImageViews[i];
		attachments[1] = m_DepthImageView;

		framebufferCreateInfo.pAttachments = &attachments[0];
		framebufferCreateInfo.attachmentCount = 2;

		VK_CALL(vkCreateFramebuffer(c_VkHardware.m_LogicalDevice, &framebufferCreateInfo, nullptr, &m_VkSwapChainFramebuffers[i]));
	}
}

VkResult vkn::VkSwapChain::AcquireNextImage(uint32_t* imageIndex)
{
	int currentFrameIndex = c_VkRenderer.m_CurrentFrameIndex;

	VK_CALL(vkWaitForFences(c_VkHardware.m_LogicalDevice, 1, &c_VkRenderer.m_InFlightFences[currentFrameIndex], VK_TRUE, UINT64_MAX));

	return vkAcquireNextImageKHR(c_VkHardware.m_LogicalDevice, m_SwapChain, UINT64_MAX, c_VkRenderer.m_ImageAvailableSemaphores[currentFrameIndex], VK_NULL_HANDLE, imageIndex);
}

VkResult vkn::VkSwapChain::SubmitCommandBuffers(const VkCommandBuffer* commandBuffers, uint32_t imageIndex)
{
	int currentFrameIndex = c_VkRenderer.m_CurrentFrameIndex;

	VkSubmitInfo submitInfo = VkSubmitInfo();
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { c_VkRenderer.m_ImageAvailableSemaphores[currentFrameIndex] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = commandBuffers;

	VkSemaphore signalSemaphores[] = { c_VkRenderer.m_RenderFinishedSemaphores[currentFrameIndex] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	// Only reset the fence if work is being submitted
	VK_CALL(vkResetFences(c_VkHardware.m_LogicalDevice, 1, &c_VkRenderer.m_InFlightFences[currentFrameIndex]));
	VK_CALL(vkQueueSubmit(c_VkHardware.m_GraphicsQueue, 1, &submitInfo, c_VkRenderer.m_InFlightFences[currentFrameIndex]));

	VkPresentInfoKHR presentationInfo = VkPresentInfoKHR();
	presentationInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentationInfo.waitSemaphoreCount = 1;
	presentationInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_SwapChain };
	presentationInfo.swapchainCount = 1;
	presentationInfo.pSwapchains = swapChains;
	presentationInfo.pImageIndices = &imageIndex;
	presentationInfo.pResults = nullptr;

	return vkQueuePresentKHR(c_VkHardware.m_PresentationQueue, &presentationInfo);
}

void vkn::VkSwapChain::Destroy()
{
	// Depth image
	vkDestroyImageView(c_VkHardware.m_LogicalDevice, m_DepthImageView, nullptr);
	vmaDestroyImage(c_VkRenderer.m_VmaAllocator, m_DepthImage.m_Image, m_DepthImage.m_Allocation);

	// Framebuffers
	for (VkFramebuffer framebuffer : m_VkSwapChainFramebuffers)
	{
		vkDestroyFramebuffer(c_VkHardware.m_LogicalDevice, framebuffer, nullptr);
	}

	// Image views
	for (auto imageView : m_SwapChainImageViews)
	{
		vkDestroyImageView(c_VkHardware.m_LogicalDevice, imageView, nullptr);
	}

	// Kill current swap chain
	vkDestroySwapchainKHR(c_VkHardware.m_LogicalDevice, m_SwapChain, nullptr);
}

///
/// Swap Chain Utilities
///

VkSurfaceFormatKHR vkn::VkSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
{
	for (const VkSurfaceFormatKHR& availableFormat : availableFormats)
	{
		// Prefer SRGB formats if available
		// Ref: https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Swap_chain
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;
	}

	return availableFormats[0];
}

VkPresentModeKHR vkn::VkSwapChain::ChooseSwapPresentationMode(const std::vector<VkPresentModeKHR>& availablePresentationModes) const
{
	for (const VkPresentModeKHR& availablePresentationMode : availablePresentationModes)
	{
		// Prefer 'MAILBOX' (triple buffering) if available
		// Sounds fast
		if (availablePresentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentationMode;
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D vkn::VkSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;

	// Swap extent is the resolution of images
	// Normally equivalent to the resolution of the window being drawn to in pixels
	// Good source of errors if images are looking weird later
	VkRendererContext* vkContext = dynamic_cast<VkRendererContext*>(m_Window->GetRendererContext());

	VkExtent2D actualExtent = vkContext->GetVulkanClientExtent();
	actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	return actualExtent;
}

vkn::SwapChainSupportDetails vkn::VkSwapChain::QuerySwapChainSupport(const VkPhysicalDevice& device) const
{
	SwapChainSupportDetails swapChainSupportDetails;

	// Query swap chain capabilities
	VK_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, c_VkHardware.m_Surface, &swapChainSupportDetails.m_SurfaceCapabilities));

	// Query surface formats 
	uint32_t surfaceFormatCount;
	VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, c_VkHardware.m_Surface, &surfaceFormatCount, nullptr));
	if (surfaceFormatCount > 0)
	{
		swapChainSupportDetails.m_SurfaceFormats.resize(surfaceFormatCount);
		VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, c_VkHardware.m_Surface, &surfaceFormatCount, swapChainSupportDetails.m_SurfaceFormats.data()));
	}

	// Query presentation modes
	uint32_t presentationModeCount;
	VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(device, c_VkHardware.m_Surface, &presentationModeCount, nullptr));
	if (presentationModeCount > 0)
	{
		swapChainSupportDetails.m_PresentationModes.resize(presentationModeCount);
		VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(device, c_VkHardware.m_Surface, &presentationModeCount, swapChainSupportDetails.m_PresentationModes.data()));
	}

	return swapChainSupportDetails;
}
