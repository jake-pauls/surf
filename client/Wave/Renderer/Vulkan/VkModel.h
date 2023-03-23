#pragma once

#include <glm/glm.hpp>

#include "VkMesh.h"
#include "VkTypes.h"

namespace vkn
{
	class VkMaterial;
	class VkRenderer;
	class VkHardware;
	class VkShaderPipeline;

	class VkModel
	{
		friend class VkRenderer;
		friend class VkHardware;
		friend class VkShaderPipeline;

	public:
		VkModel() = default;
		VkModel(const VkRenderer& renderer, const VkMesh& mesh, VkMaterial* material);
		~VkModel();

		VkModel(const VkModel& other) = delete;
		VkModel& operator=(const VkModel& other) = delete;

		void AllocateVertexBuffer();
		void AllocateIndexBuffer();
		void AllocateDescriptorSets();

		void Bind(VkCommandBuffer commandBuffer) const;
		void Draw(VkCommandBuffer commandBuffer) const;

		inline constexpr VkMesh& GetMesh() { return m_Mesh; }

		inline constexpr bool HasIndexBuffer() const { return !m_Mesh.m_Indices.empty() && m_IndexBuffer.m_Buffer && m_IndexBuffer.m_Allocation; }

	public:
		glm::mat4 m_ModelMatrix;

	private:
		const VkRenderer& c_VkRenderer;
		const VkHardware& c_VkHardware;
		VkMesh m_Mesh;
		VkMaterial* m_Material;

		std::vector<VmaAllocatedDescriptorSet> m_UniformBuffers = {};

		VmaAllocatedBuffer m_VertexBuffer = {};
		VmaAllocatedBuffer m_IndexBuffer = {};
	};
}
