#include "VkModel.h"

#include "VkMesh.h"
#include "VkTypes.h"
#include "VkHardware.h"

vkn::VkModel::VkModel(const VkHardware& hardware, const VkMesh& mesh)
	: c_VkHardware(hardware)
	, m_Mesh(mesh)
{
	AllocateVertexBuffer();
	AllocateIndexBuffer();
}

vkn::VkModel::~VkModel()
{
	vmaDestroyBuffer(c_VkHardware.m_VmaAllocator, m_VertexBuffer.m_Buffer, m_VertexBuffer.m_Allocation);
	if (HasIndexBuffer())
		vmaDestroyBuffer(c_VkHardware.m_VmaAllocator, m_IndexBuffer.m_Buffer, m_IndexBuffer.m_Allocation);
}

void vkn::VkModel::AllocateVertexBuffer()
{
	const size_t vertexBufferSize = sizeof(m_Mesh.m_Vertices[0]) * static_cast<uint32_t>(m_Mesh.m_Vertices.size());

	c_VkHardware.CreateVMABuffer(m_VertexBuffer,
		vertexBufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VMA_MEMORY_USAGE_CPU_TO_GPU);

	void* meshData;
	vmaMapMemory(c_VkHardware.m_VmaAllocator, m_VertexBuffer.m_Allocation, &meshData);
	memcpy(meshData, m_Mesh.m_Vertices.data(), vertexBufferSize);
	vmaUnmapMemory(c_VkHardware.m_VmaAllocator, m_VertexBuffer.m_Allocation);
}

void vkn::VkModel::AllocateIndexBuffer()
{
	const size_t indexBufferSize = sizeof(m_Mesh.m_Indices[0]) * static_cast<uint32_t>(m_Mesh.m_Indices.size());

	if (indexBufferSize != 0)
	{
		c_VkHardware.CreateVMABuffer(m_IndexBuffer,
			indexBufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VMA_MEMORY_USAGE_CPU_TO_GPU);

		void* meshData;
		vmaMapMemory(c_VkHardware.m_VmaAllocator, m_IndexBuffer.m_Allocation, &meshData);
		memcpy(meshData, m_Mesh.m_Indices.data(), indexBufferSize);
		vmaUnmapMemory(c_VkHardware.m_VmaAllocator, m_IndexBuffer.m_Allocation);
	}
}

void vkn::VkModel::Bind(VkCommandBuffer commandBuffer) const
{
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_VertexBuffer.m_Buffer, &offset);

	if (HasIndexBuffer())
		vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer.m_Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void vkn::VkModel::Draw(VkCommandBuffer commandBuffer) const
{
	if (HasIndexBuffer())
	{
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Mesh.m_Indices.size()), 1, 0, 0, 0);
	}
	else
	{
		vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_Mesh.m_Vertices.size()), 1, 0, 0);
	}
}
