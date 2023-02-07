#include "VkPass.h"

#include "VkRenderer.h"
#include "VkHardware.h"

vkn::VkPass::VkPass(const VkRenderer& renderer)
	: c_VkRenderer(renderer)
{
	core::Log(ELogType::Trace, "[VkPass] Creating a render pass");

	Create();
}

vkn::VkPass::~VkPass()
{
	core::Log(ELogType::Trace, "[VkPass] Destroying a render pass");

	Destroy();
}

void vkn::VkPass::Create()
{
	const VkHardware& vkHardware = c_VkRenderer.m_VkHardware;
	const VkSwapChain& vkSwapChain = c_VkRenderer.m_VkSwapChain;

	VkAttachmentDescription colorAttachment = VkAttachmentDescription();
	colorAttachment.format = vkSwapChain.m_SwapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Subpass
	VkAttachmentReference colorAttachmentRef = VkAttachmentReference();
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = VkSubpassDescription();
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	// Render pass
	VkRenderPassCreateInfo renderPassCreateInfo = VkRenderPassCreateInfo();
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &colorAttachment;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;

	VK_CALL(vkCreateRenderPass(vkHardware.m_LogicalDevice, &renderPassCreateInfo, nullptr, &m_RenderPass));
}

void vkn::VkPass::Destroy()
{
	vkDestroyRenderPass(c_VkRenderer.m_VkHardware.m_LogicalDevice, m_RenderPass, nullptr);
}