#pragma once

#include <vulkan/vulkan.h>

namespace vkn
{
	///
	/// Commands
	/// 

	/// @brief Constructs create info struct for a command pool
	VkCommandPoolCreateInfo InitCommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);

	/// @brief Constructs allocate info struct for command buffer allocation 
	VkCommandBufferAllocateInfo InitCommandBufferAllocateInfo(VkCommandPool pool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	/// @brief Constructs begin info struct for starting command buffers
	VkCommandBufferBeginInfo InitCommandBufferBeginInfo(VkCommandBufferUsageFlags flags = 0);

	/// @brief Constructs create info struct for a framebuffer
	VkFramebufferCreateInfo InitFramebufferCreateInfo(VkRenderPass renderPass, VkExtent2D extent);

	///
	/// Synchronization Objects
	/// 

	/// @brief Constructs create info struct for a VkFence
	VkFenceCreateInfo InitFenceCreateInfo(VkFenceCreateFlags flags = 0);

	VkSemaphoreCreateInfo InitSemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0);
}