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

	/// @brief Constructs an info struct for a command buffer submission
	VkSubmitInfo InitSubmitInfo(const VkCommandBuffer* commandBuffers);

	///
	/// Synchronization Objects
	/// 

	/// @brief Constructs create info struct for a VkFence
	VkFenceCreateInfo InitFenceCreateInfo(VkFenceCreateFlags flags = 0);

	/// @brief Constructs create info struct for a VkSemaphore  
	VkSemaphoreCreateInfo InitSemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0);

	///
	///	Pipeline / ShaderPipeline
	/// 

	/// @brief Constructs create info struct for a pipeline stage 
	VkPipelineShaderStageCreateInfo InitPipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);

	/// @brief Constructs a create info struct fo a vertex input state
	VkPipelineVertexInputStateCreateInfo InitPipelineVertexInputStateCreateInfo(
		const VkVertexInputBindingDescription* bindingDescriptions = VK_NULL_HANDLE, 
		uint32_t bindingDescriptionCount = 0,
		const VkVertexInputAttributeDescription* attributeDescriptions = VK_NULL_HANDLE,
		uint32_t attributeDescriptionCount = 0
	);

	/// @brief Constructs a create info struct for an input assembly 
	VkPipelineInputAssemblyStateCreateInfo InitPipelineInputAssemblyCreateInfo(VkPrimitiveTopology topology);

	/// @brief Constructs a create info struct for a rasterization state
	VkPipelineRasterizationStateCreateInfo InitPipelineRasertizationStateCreateInfo(VkPolygonMode polygonMode, VkCullModeFlags cullMode = VK_CULL_MODE_NONE);

	/// @brief Constructs a create info struct for a multisample state 
	VkPipelineMultisampleStateCreateInfo InitPipelineMultisampleStateCreateInfo();

	/// @brief Constructs a default color blend attachment
	VkPipelineColorBlendAttachmentState InitPipelineColorBlendAttachmentState();

	/// @brief Constructs a create info struct for a color blend state
	VkPipelineColorBlendStateCreateInfo InitPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendAttachmentState* colorBlendAttachment);

	/// @brief Constructs a create info struct for a pipeline layout
	VkPipelineLayoutCreateInfo InitPipelineLayoutCreateInfo();

	/// @brief Constructs a create info struct for pipeline depth testing information 
	VkPipelineDepthStencilStateCreateInfo InitPipelineDepthStencilStateCreateInfo(bool useDepthTest, bool useDepthWrite, VkCompareOp compareOp);

	/// @brief Constructs a create info struct for a pipeline's viewport state
	VkPipelineViewportStateCreateInfo InitPipelineViewportStateCreateInfo(int vieportCount = 1, int scissorCount = 1);

	///
	///	Push Constants
	/// 

	/// @brief Constructs a push constant range struct for a vertex shader
	VkPushConstantRange InitVertexPushConstantRange(const uint32_t size, const uint32_t offset = 0);

	/// @brief Constructs a push constant range struct for a fragment shader
	VkPushConstantRange InitFragmentPushConstantRange(const uint32_t size, const uint32_t offset = 0);

	///
	///	Descriptor Sets/Pools
	/// 

	/// @brief Constructs a create info struct for a descriptor pool 
	VkDescriptorPoolCreateInfo InitDescriptorPoolCreateInfo(const uint32_t poolSizeCount, const VkDescriptorPoolSize* poolSizes, int maxSets, VkDescriptorPoolCreateFlags flags = 0);

	/// @brief Constructs a layout binding for a descriptor set 
	VkDescriptorSetLayoutBinding InitDescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, int count = 1);

	/// @brief Constructs a create info struct for a descriptor set layout 
	VkDescriptorSetLayoutCreateInfo InitDescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutBinding* binding, int count = 1);

	/// @brief Constructrs an info struct for descriptor set allocation
	VkDescriptorSetAllocateInfo InitDescriptorSetAllocateInfo(const VkDescriptorPool& descriptorPool, const VkDescriptorSetLayout* descriptorSetLayouts, int count = 1);

	/// @brief Constructs an info struct for a descriptor buffer
	VkDescriptorBufferInfo InitDescriptorBufferInfo(VkBuffer& buffer, const uint32_t range);

	/// @brief Constructs an info struct for a texture sampler
	VkSamplerCreateInfo InitSamplerCreateInfo(VkFilter filters, VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);

	/// @brief Constructs a set of write information for a buffer descriptor set
	VkWriteDescriptorSet InitWriteDescriptorSetBuffer(VkDescriptorType type, VkDescriptorSet set, VkDescriptorBufferInfo* bufferInfo, uint32_t binding);

	/// @brief Constructs a set of write information for an image/sampler descriptor set
	VkWriteDescriptorSet InitWriteDescriptorSetImage(VkDescriptorType type, VkDescriptorSet set, VkDescriptorImageInfo* imageInfo, uint32_t binding);

	///
	/// Renderpass
	/// 

	VkRenderPassBeginInfo InitRenderPassBeginInfo(VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D extent);

	///
	///	Images
	/// 

	/// @brief Constructs a create info struct for an image 
	VkImageCreateInfo InitImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);

	/// @brief Constructs a create info struct for an image view
	VkImageViewCreateInfo InitImageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags flags);
}