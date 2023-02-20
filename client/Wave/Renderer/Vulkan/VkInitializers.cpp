#include "VkInitializers.h"

///
/// Commands
/// 

VkCommandPoolCreateInfo vkn::InitCommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags /* = 0 */)
{
	VkCommandPoolCreateInfo info = VkCommandPoolCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.pNext = nullptr;

	info.flags = flags;
	info.queueFamilyIndex = queueFamilyIndex;

	return info;
}

VkCommandBufferAllocateInfo vkn::InitCommandBufferAllocateInfo(
	VkCommandPool pool, 
	uint32_t count /* = 1 */, 
	VkCommandBufferLevel level /* = VK_COMMAND_BUFFER_LEVEL_PRIMARY */
)
{
	VkCommandBufferAllocateInfo info = VkCommandBufferAllocateInfo();
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.pNext = nullptr;

	info.commandPool = pool;
	info.commandBufferCount = count;
	info.level = level;

	return info;
}

VkCommandBufferBeginInfo vkn::InitCommandBufferBeginInfo(VkCommandBufferUsageFlags flags /* = 0 */)
{
	VkCommandBufferBeginInfo info = VkCommandBufferBeginInfo();
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.pNext = nullptr;

	info.pInheritanceInfo = nullptr;
	info.flags = flags;

	return info;
}

VkFramebufferCreateInfo vkn::InitFramebufferCreateInfo(VkRenderPass renderPass, VkExtent2D extent)
{
	VkFramebufferCreateInfo info = VkFramebufferCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	info.pNext = nullptr;

	info.renderPass = renderPass;
	info.attachmentCount = 1;
	info.width = extent.width;
	info.height = extent.height;
	info.layers = 1;

	return info;
}

///
/// Synchronization Objects
/// 

VkFenceCreateInfo vkn::InitFenceCreateInfo(VkFenceCreateFlags flags /* = 0 */)
{
	VkFenceCreateInfo info = VkFenceCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.pNext = nullptr;

	info.flags = flags;

	return info;
}

VkSemaphoreCreateInfo vkn::InitSemaphoreCreateInfo(VkSemaphoreCreateFlags flags /* = 0 */)
{
	VkSemaphoreCreateInfo info = VkSemaphoreCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	info.pNext = nullptr;

	info.flags = flags;

	return info;
}

///
///	Pipeline
/// 

VkPipelineShaderStageCreateInfo vkn::InitPipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule)
{
	VkPipelineShaderStageCreateInfo info = VkPipelineShaderStageCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.pNext = nullptr;

	// Shader stage for this pipeline
	info.stage = stage;
	
	// Shader for this pipeline
	info.module = shaderModule;

	// Entrypoint in the shader for this pipeline
	info.pName = "main";

	return info;
}

VkPipelineVertexInputStateCreateInfo vkn::InitPipelineVertexInputStateCreateInfo(
	const VkVertexInputBindingDescription* bindingDescriptions /* = VK_NULL_HANDLE */, 
	uint32_t bindingDescriptionCount /* = 0 */,
	const VkVertexInputAttributeDescription* attributeDescriptions /* = VK_NULL_HANDLE */,
	uint32_t attributeDescriptionCount /* = 0 */
)
{

	VkPipelineVertexInputStateCreateInfo info = VkPipelineVertexInputStateCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	info.pNext = nullptr;

	info.vertexBindingDescriptionCount = bindingDescriptionCount;
	info.pVertexBindingDescriptions = bindingDescriptions;
	info.vertexAttributeDescriptionCount = attributeDescriptionCount;
	info.pVertexAttributeDescriptions = attributeDescriptions;

	info.flags = 0;

	return info;
}

VkPipelineInputAssemblyStateCreateInfo vkn::InitPipelineInputAssemblyCreateInfo(VkPrimitiveTopology topology) 
{
	VkPipelineInputAssemblyStateCreateInfo info = VkPipelineInputAssemblyStateCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.pNext = nullptr;

	info.topology = topology;
	info.primitiveRestartEnable = VK_FALSE;

	return info;
}

VkPipelineRasterizationStateCreateInfo vkn::InitPipelineRasertizationStateCreateInfo(VkPolygonMode polygonMode, VkCullModeFlags cullMode /* = VK_CULL_MODE_NONE */)
{
	VkPipelineRasterizationStateCreateInfo info = VkPipelineRasterizationStateCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	info.pNext = nullptr;

	info.depthClampEnable = VK_FALSE;
	info.rasterizerDiscardEnable = VK_FALSE;    // Set this to VK_TRUE to disable output to framebuffer 
	info.polygonMode = polygonMode;				// This can change how fragments are generated for geometry
	info.lineWidth = 1.0f;

	info.cullMode = cullMode;
	info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	// No depth bias
	info.depthBiasEnable = VK_FALSE;
	info.depthBiasConstantFactor = 0.0f;
	info.depthBiasClamp = 0.0f;
	info.depthBiasSlopeFactor = 0.0f;

	return info;
}

VkPipelineMultisampleStateCreateInfo vkn::InitPipelineMultisampleStateCreateInfo()
{
	VkPipelineMultisampleStateCreateInfo info = VkPipelineMultisampleStateCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.pNext = nullptr;

	info.sampleShadingEnable = VK_FALSE;
	info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	info.minSampleShading = 1.0f;
	info.pSampleMask = nullptr;
	info.alphaToCoverageEnable = VK_FALSE;
	info.alphaToOneEnable = VK_FALSE;

	return info;
}

VkPipelineColorBlendAttachmentState vkn::InitPipelineColorBlendAttachmentState()
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment = VkPipelineColorBlendAttachmentState();

	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT 
		| VK_COLOR_COMPONENT_G_BIT 
		| VK_COLOR_COMPONENT_B_BIT 
		| VK_COLOR_COMPONENT_A_BIT;

	// Default set to alpha blending
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	return colorBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo vkn::InitPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendAttachmentState* colorBlendAttachment)
{
	VkPipelineColorBlendStateCreateInfo info = VkPipelineColorBlendStateCreateInfo();

	info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	info.logicOpEnable = VK_FALSE;
	info.logicOp = VK_LOGIC_OP_COPY; 	
	info.attachmentCount = 1;
	info.pAttachments = colorBlendAttachment;

	info.blendConstants[0] = 0.0f;
	info.blendConstants[1] = 0.0f;
	info.blendConstants[2] = 0.0f;
	info.blendConstants[3] = 0.0f;

	return info;
}

VkPipelineLayoutCreateInfo vkn::InitPipelineLayoutCreateInfo()
{
	VkPipelineLayoutCreateInfo info = VkPipelineLayoutCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;

	info.flags = 0;
	info.setLayoutCount = 0;
	info.pSetLayouts = nullptr;
	info.pushConstantRangeCount = 0;
	info.pPushConstantRanges = nullptr;

	return info;
}

VkPipelineDepthStencilStateCreateInfo vkn::InitPipelineDepthStencilStateCreateInfo(bool useDepthTest, bool useDepthWrite, VkCompareOp compareOp)
{
	VkPipelineDepthStencilStateCreateInfo info = VkPipelineDepthStencilStateCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	info.pNext = nullptr;
	
	info.depthTestEnable = useDepthTest ? VK_TRUE : VK_FALSE;
	info.depthWriteEnable = useDepthWrite ? VK_TRUE : VK_FALSE;
	info.depthCompareOp = useDepthTest ? compareOp : VK_COMPARE_OP_ALWAYS;

	info.depthBoundsTestEnable = VK_FALSE;
	info.minDepthBounds = 0.0f;
	info.maxDepthBounds = 1.0f;
	info.stencilTestEnable = VK_FALSE;

	return info;
}

VkPipelineViewportStateCreateInfo vkn::InitPipelineViewportStateCreateInfo(int vieportCount /* = 1 */, int scissorCount /* = 1 */)
{
	VkPipelineViewportStateCreateInfo info = VkPipelineViewportStateCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	info.pNext = nullptr;

	info.viewportCount = 1;
	info.scissorCount = 1;

	return info;
}

///
///	Push Constants
/// 

VkPushConstantRange vkn::InitPushConstantRange(const uint32_t size)
{
	VkPushConstantRange range = VkPushConstantRange();
	range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	range.size = size;
	range.offset = 0;
	
	return range;
}

///
///	Descriptor Sets/Pools
/// 

VkDescriptorPoolCreateInfo vkn::InitDescriptorPoolCreateInfo(const uint32_t poolSizeCount, 
	const VkDescriptorPoolSize* poolSizes,
	int maxSets)
{
	VkDescriptorPoolCreateInfo info = VkDescriptorPoolCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.flags = 0;

	info.maxSets = maxSets;
	info.poolSizeCount = poolSizeCount;
	info.pPoolSizes = poolSizes;

	return info;
}

VkDescriptorSetLayoutBinding vkn::InitDescriptorSetLayoutBinding(VkDescriptorType type, 
	VkShaderStageFlags stageFlags,
	int count /* = 1 */)
{
	VkDescriptorSetLayoutBinding binding = VkDescriptorSetLayoutBinding();
	binding.binding = 0;

	binding.descriptorType = type;
	binding.stageFlags = stageFlags;
	binding.descriptorCount = count;

	return binding;
}

VkDescriptorSetLayoutCreateInfo vkn::InitDescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutBinding* binding, int count /* = 1 */)
{
	VkDescriptorSetLayoutCreateInfo info = VkDescriptorSetLayoutCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;

	info.pBindings = binding;
	info.bindingCount = count;

	return info;
}

VkDescriptorSetAllocateInfo vkn::InitDescriptorSetAllocateInfo(const VkDescriptorPool& descriptorPool, 
	const VkDescriptorSetLayout* descriptorSetLayouts, 
	int count /* = 1 */)
{
	VkDescriptorSetAllocateInfo info = VkDescriptorSetAllocateInfo();
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	info.pNext = nullptr;

	info.descriptorPool = descriptorPool;
	info.pSetLayouts = descriptorSetLayouts;
	info.descriptorSetCount = 1;

	return info;
}

VkDescriptorBufferInfo vkn::InitDescriptorBufferInfo(VkBuffer& buffer, const uint32_t range)
{
	VkDescriptorBufferInfo info = VkDescriptorBufferInfo();

	info.buffer = buffer;
	info.offset = 0;
	info.range = range;

	return info;
}

///  
/// Renderpass
///

VkRenderPassBeginInfo vkn::InitRenderPassBeginInfo(VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D extent)
{
	VkRenderPassBeginInfo info = VkRenderPassBeginInfo();
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.pNext = nullptr;

	info.renderPass = renderPass;
	info.framebuffer = framebuffer;
	info.renderArea.extent = extent;

	info.renderArea.offset = { 0, 0 };

	return info;
}

///
///	Images
/// 

VkImageCreateInfo vkn::InitImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent)
{
	VkImageCreateInfo info = VkImageCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	info.pNext = nullptr;

	info.imageType = VK_IMAGE_TYPE_2D;

	info.format = format;
	info.extent = extent;

	info.mipLevels = 1;
	info.arrayLayers = 1;
	info.samples = VK_SAMPLE_COUNT_1_BIT;
	info.tiling = VK_IMAGE_TILING_OPTIMAL;
	info.usage = usageFlags;

	return info;
}

VkImageViewCreateInfo vkn::InitImageViewCreateInfo(VkFormat format, 
	VkImage image, 
	VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo info = VkImageViewCreateInfo();
	info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	info.pNext = nullptr;

	info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	info.image = image;
	info.format = format;

	info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	info.subresourceRange.aspectMask = aspectFlags;
	info.subresourceRange.baseMipLevel = 0;
	info.subresourceRange.levelCount = 1;
	info.subresourceRange.baseArrayLayer = 0;
	info.subresourceRange.layerCount = 1;

	return info;
}
