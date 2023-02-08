#include "VkInitializers.h"

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
