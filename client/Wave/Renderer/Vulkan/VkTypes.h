#pragma once

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

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