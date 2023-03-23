#pragma once

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "VkTypes.h"
#include "VkShaderPipeline.h"

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
		glm::vec2 m_UV;

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

		void LoadFromObj(const char* filename);
	};

    struct VkMeshPushConstants
    {
		glm::vec4 m_CameraPosition;
        glm::mat4 m_ModelMatrix;
    };

	/// @brief Represents the uniforms passed to the default UBO in shaders
	/// @note Vectors are padded together to reduce the complexity of the descriptor sets
	struct VkMeshUniformBufferObject
	{
		// PBR
		glm::vec4 m_LightPosition;
		glm::vec4 m_LightColor;
		glm::vec4 m_Albedo;
		// x - metallic, y - roughness, z - ao, w - unused
		glm::vec4 m_PBRSettings;
		// View
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
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
