#include "VkPass.h"

#include "VkRenderer.h"
#include "VkHardware.h"

vkn::VkPass::VkPass(const VkDevice& device, const VkSwapChain& swapChain)
	: c_LogicalDevice(device)
	, c_VkSwapChain(swapChain)
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
	// Color Attachment
	VkAttachmentDescription colorAttachment = VkAttachmentDescription();
	colorAttachment.format = c_VkSwapChain.m_SwapChainImageFormat;
	colorAttachment.flags = 0;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = VkAttachmentReference();
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Depth Attachment
	VkAttachmentDescription depthAttachment = VkAttachmentDescription();
	depthAttachment.flags = 0;
	depthAttachment.format = c_VkSwapChain.m_DepthFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = VkAttachmentReference();
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription attachments[2] = {
		colorAttachment,
		depthAttachment
	};

	// Subpass
	VkSubpassDescription subpass = VkSubpassDescription();
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	// Subpass dependencies
	VkSubpassDependency subpassDependency = VkSubpassDependency();
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = 0;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency depthDependency = VkSubpassDependency();
	depthDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	depthDependency.dstSubpass = 0;
	depthDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT 
		| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT 
		| VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	depthDependency.srcAccessMask = 0;
	depthDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	depthDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT 
		| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT 
		| VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

	VkSubpassDependency dependencies[2] = {
		subpassDependency,
		depthDependency
	};

	// Render pass
	VkRenderPassCreateInfo renderPassCreateInfo = VkRenderPassCreateInfo();
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = 2;
	renderPassCreateInfo.pAttachments = attachments;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 2;
	renderPassCreateInfo.pDependencies = dependencies;

	VK_CALL(vkCreateRenderPass(c_LogicalDevice, &renderPassCreateInfo, nullptr, &m_RenderPass));
}

void vkn::VkPass::Destroy()
{
	vkDestroyRenderPass(c_LogicalDevice, m_RenderPass, nullptr);
}