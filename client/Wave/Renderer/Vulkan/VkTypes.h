#pragma once

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

// TODO: Abstract Vulkan utilities
namespace vkn
{
	/// @brief Contains data for a buffer allocated using VMA 
	struct VmaAllocatedBuffer
	{
		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;
	};

	/// @brief Contains data for an image allocated using VMA
	struct VmaAllocatedImage
	{
		VkImage m_Image;
		VmaAllocation m_Allocation;
	};

	void CreateBuffer(VmaAllocatedBuffer* buffer, 
		size_t size, 
		VkBufferUsageFlags usageFlags, 
		VmaMemoryUsage memoryUsage);
}
