#pragma once

#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "Vulkan/VkTypes.h"

namespace vkn
{
	struct VkVertexInputDescription
	{
		std::vector<VkVertexInputBindingDescription> m_Bindings;
		std::vector<VkVertexInputAttributeDescription> m_Attributes;
		VkPipelineVertexInputStateCreateFlags m_Flags = 0;
	};

	struct VkVertex
	{
		glm::vec3 m_Position;
		glm::vec3 m_Normal;
		glm::vec3 m_Color;

		static VkVertexInputDescription GetVertexInputDescription();

		bool operator==(const VkVertex& other) const 
		{
			return m_Position == other.m_Position && m_Normal == other.m_Normal && m_Color == other.m_Color;
		}
	};

	struct VkMesh
	{
		std::vector<VkVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		VmaAllocatedBuffer m_VertexBuffer;
		VmaAllocatedBuffer m_IndexBuffer;

		bool LoadFromObj(const char* filename);

		inline bool HasIndexBuffer() const { return !m_Indices.empty() && m_IndexBuffer.m_Buffer && m_IndexBuffer.m_Allocation; }
	};

    struct VkMeshPushConstants
    {
        glm::vec4 m_Data;
        glm::mat4 m_MvpMatrix;
    };
}

namespace std
{
	template<> 
	struct hash<vkn::VkVertex> 
	{
		size_t operator()(vkn::VkVertex const& vertex) const
		{
			size_t seed = 0;
			core::HashCombine(seed, vertex.m_Position, vertex.m_Normal, vertex.m_Color);
			return seed;
		}
	};
}
