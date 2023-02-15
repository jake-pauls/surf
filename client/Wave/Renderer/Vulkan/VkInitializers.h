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

	/// @brief Constructs create info struct for a VkSemaphore  
	VkSemaphoreCreateInfo InitSemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0);

	///
	///	Pipeline
	/// 

	/// @brief Constructs create info struct for a pipeline stage 
	VkPipelineShaderStageCreateInfo InitPipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);

	/// @brief Constructs a create info struct fo a vertex input state
	VkPipelineVertexInputStateCreateInfo InitPipelineVertexInputStateCreateInfo();

	/// @brief Constructs a create info struct for an input assembly 
	VkPipelineInputAssemblyStateCreateInfo InitPipelineInputAssemblyCreateInfo(VkPrimitiveTopology topology);

	/// @brief Constructs a create info struct for a rasterization state
	VkPipelineRasterizationStateCreateInfo InitPipelineRasertizationStateCreateInfo(VkPolygonMode polygonMode, VkCullModeFlags cullMode = VK_CULL_MODE_NONE);

	/// @brief Constructs a create info struct for a multisample state 
	VkPipelineMultisampleStateCreateInfo InitPipelineMultisampleStateCreateInfo();

	/// @brief Constructs a default color blend attachment
	VkPipelineColorBlendAttachmentState InitPipelineColorBlendAttachmentState();

	/// @brief Constructs a create info struct for a color blend state
	VkPipelineColorBlendStateCreateInfo InitPipelineColorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState colorBlendAttachment);

	/// @brief Constructs a create info struct for a pipeline layout
	VkPipelineLayoutCreateInfo InitPipelineLayoutCreateInfo();
}