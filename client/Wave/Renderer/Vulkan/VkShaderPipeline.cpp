#include "VkShaderPipeline.h"

#include "Window.h"
#include "VkPass.h"
#include "VkRenderer.h"
#include "VkRendererContext.h"

vkn::VkShaderPipeline::VkShaderPipeline(
	const VkDevice& device, 
	const VkRenderPass& renderPass, 
	const std::string& vertexShader, 
	const std::string& fragmentShader
) 
	: wv::Shader(vertexShader, fragmentShader)
	, c_LogicalDevice(device)
	, c_RenderPass(renderPass)
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
	m_VertexShaderModule = LoadShaderModule(m_VertexShaderFile);
	m_FragmentShaderModule = LoadShaderModule(m_FragmentShaderFile);

	// Pipeline shader stages
	VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = VkPipelineShaderStageCreateInfo();
	vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageCreateInfo.module = m_VertexShaderModule;
	vertexShaderStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo = VkPipelineShaderStageCreateInfo();
	fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderStageCreateInfo.module = m_FragmentShaderModule;
	fragmentShaderStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {
		vertexShaderStageCreateInfo,
		fragmentShaderStageCreateInfo
	};

	// Vertex input
	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = VkPipelineVertexInputStateCreateInfo();
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;
	vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;

	// Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = VkPipelineInputAssemblyStateCreateInfo();
	inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

	// Viewport state
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = VkPipelineViewportStateCreateInfo();
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.scissorCount = 1;

	// Rasterizer - turns geometry into the fragments that are colored by the fragment shader
	VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = VkPipelineRasterizationStateCreateInfo();
	rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerCreateInfo.depthClampEnable = VK_FALSE;
	rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE; // Set this to VK_TRUE to disable output to framebuffer
	rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL; // This can change how fragments are generated for geometry
	rasterizerCreateInfo.lineWidth = 1.0f;
	rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizerCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizerCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizerCreateInfo.depthBiasClamp = 0.0f;
	rasterizerCreateInfo.depthBiasSlopeFactor = 0.0f;

	// Mulisampling - can perform anti-aliasing (combining multiple fragment shader results into one pixel)
	VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo = VkPipelineMultisampleStateCreateInfo();
	multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;
	multisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisamplingCreateInfo.minSampleShading = 1.0f;
	multisamplingCreateInfo.pSampleMask = nullptr;
	multisamplingCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multisamplingCreateInfo.alphaToOneEnable = VK_FALSE;

	// Depth and stencil testing could be done here

	// Color blending - fragment shader returns a color, that color has to be combined with a color in the framebuffer
	VkPipelineColorBlendAttachmentState colorBlendAttachment = VkPipelineColorBlendAttachmentState();
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	// This modifies the blend function, default is set to alpha blending
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = VkPipelineColorBlendStateCreateInfo();
	colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendCreateInfo.logicOp = VK_LOGIC_OP_COPY; // TODO: This may have to be set to a bitwise method
	colorBlendCreateInfo.attachmentCount = 1;
	colorBlendCreateInfo.pAttachments = &colorBlendAttachment;
	colorBlendCreateInfo.blendConstants[0] = 0.0f;
	colorBlendCreateInfo.blendConstants[1] = 0.0f;
	colorBlendCreateInfo.blendConstants[2] = 0.0f;
	colorBlendCreateInfo.blendConstants[3] = 0.0f;

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

	// Pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = VkPipelineLayoutCreateInfo();
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 0;
	pipelineLayoutCreateInfo.pSetLayouts = nullptr;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	VK_CALL(vkCreatePipelineLayout(c_LogicalDevice, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout));

	// Actual pipeline definition
	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = VkGraphicsPipelineCreateInfo();
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.stageCount = 2;
	graphicsPipelineCreateInfo.pStages = shaderStages;

	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
	graphicsPipelineCreateInfo.layout = m_PipelineLayout;

	// TODO: sus
	graphicsPipelineCreateInfo.renderPass = c_RenderPass;

	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	VK_CALL(vkCreateGraphicsPipelines(c_LogicalDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &m_GraphicsPipeline));

	// Cleanup shader modules once pipeline is created
	vkDestroyShaderModule(c_LogicalDevice, m_FragmentShaderModule, nullptr);
	vkDestroyShaderModule(c_LogicalDevice, m_VertexShaderModule, nullptr);
}

void vkn::VkShaderPipeline::Destroy()
{
	vkDestroyPipeline(c_LogicalDevice, m_GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(c_LogicalDevice, m_PipelineLayout, nullptr);
}

void vkn::VkShaderPipeline::Bind()
{
}

void vkn::VkShaderPipeline::Unbind()
{
}

VkShaderModule vkn::VkShaderPipeline::LoadShaderModule(const std::string& shaderFileName)
{
	std::vector<char> shaderBinaryData = core::FileHelpers::ReadBinaryFile(shaderFileName);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = VkShaderModuleCreateInfo();
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = shaderBinaryData.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderBinaryData.data());

	VkShaderModule vkShaderModule;
	VK_CALL(vkCreateShaderModule(c_LogicalDevice, &shaderModuleCreateInfo, nullptr, &vkShaderModule));

	return vkShaderModule;
}
