#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "VkTypes.h"

namespace wv
{
	class Window;
}

namespace vkn
{
	class VkPass;
	class VkRenderer;
	class VkHardware;

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR m_SurfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> m_SurfaceFormats;
		std::vector<VkPresentModeKHR> m_PresentationModes;
	};

	/// @brief Represents the swap chain for a VkRenderer
	///		   Essentially a queue of images waiting to be on the screen
	class VkSwapChain final
	{
		friend class VkPass;
		friend class VkRenderer;
		friend class VkHardware;
		friend class VkRenderer;

	public:
		explicit VkSwapChain(const VkRenderer& renderer, const VkHardware& hardware, wv::Window* window);
		~VkSwapChain();

		void RecreateSwapchain();
		VkResult AcquireNextImage(uint32_t* imageIndex);
		VkResult SubmitCommandBuffers(const VkCommandBuffer* commandBuffers, uint32_t imageIndex);

		void CreateFramebuffers();

	private:
		void CreateSwapchain();
		void CreateImageViews();
		void CreateDepthImage();

		void Destroy();

	private:
		/// @brief Selects the swap surface format from a list of available formats
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;

		/// @brief Selects the swap chain presentation mode
		/// -- defaults: VK_PRESENT_MODE_MAILBOX_KHR (triple buffering) for devices where it exists 
		///				 VK_PRESENT_MODE_FIFO_KHR (hard VSync) for most devices
		VkPresentModeKHR ChooseSwapPresentationMode(const std::vector<VkPresentModeKHR>& availablePresentationModes) const;

		/// @brief Selects the swap extent from the renderer context if not specified by passed capabilities list
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

		/// @brief Queries swap chain capabilites, surface formats, and presentation modes
		/// @param device The VkPhysicalDevice in use to query swap chain support for
		/// @return Struct containing the details for swap chain support
		SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device) const;

	private:
		wv::Window* m_Window = nullptr;
		const VkRenderer& c_VkRenderer;
		const VkHardware& c_VkHardware;

		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
		std::vector<VkImage> m_SwapChainImages = {};
		std::vector<VkImageView> m_SwapChainImageViews = {};
		std::vector<VkFramebuffer> m_VkSwapChainFramebuffers = {};

		VkFormat m_SwapChainImageFormat = VK_FORMAT_UNDEFINED;
		VkExtent2D m_SwapChainExtent = {};

		VkFormat m_DepthFormat = {};
		VmaAllocatedImage m_DepthImage = {};
		VkImageView m_DepthImageView = VK_NULL_HANDLE;
	};
}
