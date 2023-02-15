#pragma once

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

/// @brief Contains data for a buffer allocated using VMA 
struct VmaAllocatedBuffer
{
    VkBuffer m_Buffer;
    VmaAllocation m_Allocation;
};