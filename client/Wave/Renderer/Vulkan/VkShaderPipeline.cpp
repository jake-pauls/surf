#include "VkShaderPipeline.h"

#include "VkHardware.h"

vkn::VkShaderPipeline::VkShaderPipeline(const VkHardware& hardware, const std::string& vertexShader, const std::string& fragmentShader)
	: wv::Shader(vertexShader, fragmentShader)
	, c_VkHardware(hardware)
{
	core::Log(ELogType::Trace, "[VkShaderPipeline] Creating shader pipeline");

	m_VertexShaderModule = LoadShaderModule(vertexShader);
	m_FragmentShaderModule = LoadShaderModule(fragmentShader);

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
}

vkn::VkShaderPipeline::~VkShaderPipeline()
{
	core::Log(ELogType::Trace, "[VkShaderPipeline] Destroying shader pipeline");

	vkDestroyShaderModule(c_VkHardware.m_LogicalDevice, m_FragmentShaderModule, nullptr);
	vkDestroyShaderModule(c_VkHardware.m_LogicalDevice, m_VertexShaderModule, nullptr);
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
	VK_CALL(vkCreateShaderModule(c_VkHardware.m_LogicalDevice, &shaderModuleCreateInfo, nullptr, &vkShaderModule));

	return vkShaderModule;
}
