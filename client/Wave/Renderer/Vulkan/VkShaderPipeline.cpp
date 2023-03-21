#include "VkShaderPipeline.h"

#include "Window.h"
#include "VkMesh.h"
#include "VkPass.h"
#include "VkRenderer.h"
#include "VkHardware.h"
#include "VkRendererContext.h"
#include "VkInitializers.h"

vkn::VkShaderPipeline::VkShaderPipeline(const VkRenderer& renderer,
	const VkHardware& hardware,
	const std::string& vertexShader, 
	const std::string& fragmentShader,
	const int textureCount /* = 0 */) 
	: wv::Shader(vertexShader, fragmentShader)
	, c_VkRenderer(renderer)
	, c_VkHardware(hardware)
	, c_RenderPass(renderer.m_DefaultPass->m_RenderPass)
	, m_PipelineTextureCount(textureCount)
{
	core::Log(ELogType::Trace, "[VkShaderPipeline] Creating shader pipeline");

	Create();
}

vkn::VkShaderPipeline::~VkShaderPipeline()
{
	core::Log(ELogType::Trace, "[VkShaderPipeline] Destroying shader pipeline");
	
	Destroy();
}

void vkn::VkShaderPipeline::Create()
{
	// Load shaders
	VkShaderModule vertexShaderModule = LoadShaderModule(m_VertexShaderFile);
	VkShaderModule fragmentShaderModule = LoadShaderModule(m_FragmentShaderFile);

	// Pipeline shader stages
	m_ShaderStages = {
		vkn::InitPipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShaderModule),
		vkn::InitPipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShaderModule)
	};

	// Vertex attribute/binding descriptions
	VkVertexInputDescription vertexInputDescription = VkVertex::GetVertexInputDescription();
	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = vkn::InitPipelineVertexInputStateCreateInfo(
		vertexInputDescription.m_Bindings.data(),
		static_cast<uint32_t>(vertexInputDescription.m_Bindings.size()),
		vertexInputDescription.m_Attributes.data(),
		static_cast<uint32_t>(vertexInputDescription.m_Attributes.size())
	);

	// Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = vkn::InitPipelineInputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	// Rasterizer - turns geometry into the fragments that are colored by the fragment shader
	VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = vkn::InitPipelineRasertizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT);

	// Mulisampling - can perform anti-aliasing (combining multiple fragment shader results into one pixel)
	VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo = vkn::InitPipelineMultisampleStateCreateInfo();

	// Depth stencil - sets up depth testing/sampling
	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = vkn::InitPipelineDepthStencilStateCreateInfo(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

	// Color blending - fragment shader returns a color, that color has to be combined with a color in the framebuffer
	VkPipelineColorBlendAttachmentState colorBlendAttachment = vkn::InitPipelineColorBlendAttachmentState();
	VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = vkn::InitPipelineColorBlendStateCreateInfo(&colorBlendAttachment);

	// Viewport state
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = vkn::InitPipelineViewportStateCreateInfo();

	// Setup dynamic states for viewport/scissor
	// This can be manually defined to create different viewport states
	std::vector<VkDynamicState> dynamicStates = { 
		VK_DYNAMIC_STATE_VIEWPORT, 
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = VkPipelineDynamicStateCreateInfo();
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

	// Initialize push constants 
	VkPushConstantRange pushConstantRange = vkn::InitVertexPushConstantRange(sizeof(VkMeshPushConstants));
	pushConstantRange.stageFlags = (VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

	// Initialize descriptor sets/pools
	std::vector<VkDescriptorPoolSize> descriptorPoolSizes = {
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 },
	};

	// Add sampler descriptor set if pipeline is textured
	if (m_PipelineTextureCount > 0)
	{
		descriptorPoolSizes.push_back({ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10 });
	}

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = vkn::InitDescriptorPoolCreateInfo(static_cast<uint32_t>(descriptorPoolSizes.size()), descriptorPoolSizes.data(), 10);
	VK_CALL(vkCreateDescriptorPool(c_VkHardware.m_LogicalDevice, &descriptorPoolCreateInfo, nullptr, &m_DescriptorPool));

	// Create basic UBO
	VkDescriptorSetLayoutBinding uboLayoutBinding = vkn::InitDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = vkn::InitDescriptorSetLayoutCreateInfo(&uboLayoutBinding);
	VK_CALL(vkCreateDescriptorSetLayout(c_VkHardware.m_LogicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_DescriptorSetLayout));

	if (m_PipelineTextureCount > 0)
	{
		m_TextureSetLayouts.resize(m_PipelineTextureCount);

		for (size_t i = 0; i < m_TextureSetLayouts.size(); ++i)
		{
			VkDescriptorSetLayoutBinding texUniformBinding = vkn::InitDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
			VkDescriptorSetLayoutCreateInfo texDescriptorSetLayoutCreateInfo = vkn::InitDescriptorSetLayoutCreateInfo(&texUniformBinding);
			VK_CALL(vkCreateDescriptorSetLayout(c_VkHardware.m_LogicalDevice, &texDescriptorSetLayoutCreateInfo, nullptr, &m_TextureSetLayouts[i]));
		}
	}

	// Pipeline layout w/ push constants and descriptor sets
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = vkn::InitPipelineLayoutCreateInfo();

	// Append push constant ranges
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

	// Add texture set layout if pipeline is textured
	// This is kept separate to allow for easy access of texture set layouts in 'VkMaterial'
	std::vector<VkDescriptorSetLayout> setLayouts = { m_DescriptorSetLayout };
	if (m_PipelineTextureCount > 0)
	{
		for (size_t i = 0; i < m_PipelineTextureCount; ++i)
		{
			setLayouts.push_back(m_TextureSetLayouts[i]);
		}
	}

	pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
	pipelineLayoutCreateInfo.pSetLayouts = setLayouts.data();

	VK_CALL(vkCreatePipelineLayout(c_VkHardware.m_LogicalDevice, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout));

	// Actual pipeline definition
	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = VkGraphicsPipelineCreateInfo();
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(m_ShaderStages.size());
	graphicsPipelineCreateInfo.pStages = m_ShaderStages.data();
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
	graphicsPipelineCreateInfo.layout = m_PipelineLayout;
	graphicsPipelineCreateInfo.renderPass = c_RenderPass;
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	VK_CALL(vkCreateGraphicsPipelines(c_VkHardware.m_LogicalDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &m_GraphicsPipeline));

	// Cleanup shader modules once pipeline is created
	vkDestroyShaderModule(c_VkHardware.m_LogicalDevice, fragmentShaderModule, nullptr);
	vkDestroyShaderModule(c_VkHardware.m_LogicalDevice, vertexShaderModule, nullptr);
}

void vkn::VkShaderPipeline::Destroy()
{
	for (size_t i = 0; i < m_TextureSetLayouts.size(); ++i)
	{
		vkDestroyDescriptorSetLayout(c_VkHardware.m_LogicalDevice, m_TextureSetLayouts[i], nullptr);
	}

	vkDestroyDescriptorSetLayout(c_VkHardware.m_LogicalDevice, m_DescriptorSetLayout, nullptr);
	vkDestroyDescriptorPool(c_VkHardware.m_LogicalDevice, m_DescriptorPool, nullptr);

	vkDestroyPipelineLayout(c_VkHardware.m_LogicalDevice, m_PipelineLayout, nullptr);
	vkDestroyPipeline(c_VkHardware.m_LogicalDevice, m_GraphicsPipeline, nullptr);
}

void vkn::VkShaderPipeline::Bind(VkCommandBuffer commandBuffer) const
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
}

VkShaderModule vkn::VkShaderPipeline::LoadShaderModule(const std::string& shaderFileName)
{
	std::vector<char> shaderBinaryData = core::FileHelpers::ReadBinaryFile(shaderFileName);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = VkShaderModuleCreateInfo();
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = shaderBinaryData.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderBinaryData.data());

	VkShaderModule vkShaderModule;
	VK_CALL(vkCreateShaderModule(c_VkHardware.m_LogicalDevice, &shaderModuleCreateInfo, nullptr, &vkShaderModule));

	return vkShaderModule;
}
