#include "VkSwapChain.h"

#include <cmath>
#include <algorithm>

#include "Window.h"
#include "VkRenderer.h"
#include "VkHardware.h"
#include "VkRendererContext.h"
#include "VkInitializers.h"

vkn::VkSwapChain::VkSwapChain(
	wv::Window* window, 
	const VkRenderer& renderer, 
	const VkHardware& hardware
)
	: m_Window(window)
	, c_VkRenderer(renderer)
	, c_VkHardware(hardware)
{
	core::Log(ELogType::Trace, "[VkSwapChain] Creating a swap chain");

	CreateSwapchain();
	CreateImageViews();
}

vkn::VkSwapChain::~VkSwapChain()
{
	core::Log(ELogType::Trace, "[VkSwapChain] Destroying swap chain");

	Destroy();
}

void vkn::VkSwapChain::RecreateSwapchain()
{
	core::Log(ELogType::Warn, "Recreating a swapchain");

	VK_CALL(vkDeviceWaitIdle(c_VkHardware.m_LogicalDevice));

	// Cleanup swap chain before recreation
	Destroy();

	CreateSwapchain();
	CreateImageViews();
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

	// TODO: Learn about more elaborate methods for changing/rebuilding swap chains at runtime
	//	     ex: Window resizing...
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
		VkImageViewCreateInfo imageViewCreateInfo = VkImageViewCreateInfo();
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = m_SwapChainImages[i];
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = m_SwapChainImageFormat;

		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		VK_CALL(vkCreateImageView(c_VkHardware.m_LogicalDevice, &imageViewCreateInfo, nullptr, &m_SwapChainImageViews[i]));
	}
}

void vkn::VkSwapChain::CreateFramebuffers()
{
	const std::vector<VkImageView>& swapChainImageViews = m_SwapChainImageViews;

	m_VkSwapChainFramebuffers.resize(swapChainImageViews.size());

	VkFramebufferCreateInfo framebufferCreateInfo = vkn::InitFramebufferCreateInfo(c_VkRenderer.m_DefaultPass->m_RenderPass, m_SwapChainExtent);
	for (size_t i = 0; i < swapChainImageViews.size(); ++i)
	{
		framebufferCreateInfo.pAttachments = &swapChainImageViews[i];
		VK_CALL(vkCreateFramebuffer(c_VkHardware.m_LogicalDevice, &framebufferCreateInfo, nullptr, &m_VkSwapChainFramebuffers[i]));
	}
}

void vkn::VkSwapChain::Destroy()
{
	for (VkFramebuffer framebuffer : m_VkSwapChainFramebuffers)
	{
		vkDestroyFramebuffer(c_VkHardware.m_LogicalDevice, framebuffer, nullptr);
	}

	for (auto imageView : m_SwapChainImageViews)
	{
		vkDestroyImageView(c_VkHardware.m_LogicalDevice, imageView, nullptr);
	}

	// Kill current swap chain
	// TODO: Segfault here on Linux... possible vk spec issue?
	vkDestroySwapchainKHR(c_VkHardware.m_LogicalDevice, m_SwapChain, nullptr);
}

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
