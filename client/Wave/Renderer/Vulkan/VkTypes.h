#pragma once

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

namespace vkn
{
	/// @brief Contains data for a buffer allocated using VMA 
	struct VmaAllocatedBuffer
	{
		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;
	};

	/// @brief Contains data for a descriptor set allocated using VMA
	struct VmaAllocatedDescriptorSet : VmaAllocatedBuffer
	{
		VkDescriptorSet m_Descriptor;
	};

	/// @brief Contains data for an image allocated using VMA
	struct VmaAllocatedImage : VmaAllocatedBuffer
	{
		VkImage m_Image;
		VkImageView m_ImageView;
	};
}

