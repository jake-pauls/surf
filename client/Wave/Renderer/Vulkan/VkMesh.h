#pragma once

#include <vector>

#include <glm/glm.hpp>

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
	};

	struct VkMesh
	{
		std::vector<VkVertex> m_Vertices;
		VmaAllocatedBuffer m_VertexBuffer;

		bool LoadFromObj(const char* filename);
	};

    struct VkMeshPushConstants
    {
        glm::vec4 m_Data;
        glm::mat4 m_MvpMatrix;
    };
}