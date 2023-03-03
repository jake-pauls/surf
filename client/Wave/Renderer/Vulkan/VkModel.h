#pragma once

#include "VkMesh.h"
#include "VkTypes.h"

namespace vkn
{
	class VkRenderer;
	class VkHardware;

	class VkModel
	{
		friend class VkRenderer;
		friend class VkHardware;

	public:
		VkModel(const VkRenderer& renderer, const VkMesh& mesh);
		~VkModel();

		VkModel(const VkModel& other) = delete;
		VkModel& operator=(const VkModel& other) = delete;

		void AllocateVertexBuffer();
		void AllocateIndexBuffer();

		void Bind(VkCommandBuffer commandBuffer) const;
		void Draw(VkCommandBuffer commandBuffer) const;

		inline constexpr VkMesh& GetMesh() { return m_Mesh; }

		inline constexpr bool HasIndexBuffer() const { return !m_Mesh.m_Indices.empty() && m_IndexBuffer.m_Buffer && m_IndexBuffer.m_Allocation; }

	private:
		const VkRenderer& c_VkRenderer;
		const VkHardware& c_VkHardware;
		VkMesh m_Mesh;

		VmaAllocatedBuffer m_VertexBuffer = {};
		VmaAllocatedBuffer m_IndexBuffer = {};
	};
}
