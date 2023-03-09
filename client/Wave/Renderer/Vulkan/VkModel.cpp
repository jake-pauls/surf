#include "VkModel.h"

#include "VkMesh.h"
#include "VkTypes.h"
#include "VkRenderer.h"
#include "VkHardware.h"

vkn::VkModel::VkModel(const VkRenderer& renderer, const VkMesh& mesh)
	: c_VkRenderer(renderer)
	, c_VkHardware(*renderer.m_VkHardware)
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

	VmaAllocatedBuffer stagingBuffer;
	
	// Create CPU side staging buffer
	c_VkHardware.CreateVMABuffer(stagingBuffer,
		vertexBufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_ONLY);

	void* meshData;
	VK_CALL(vmaMapMemory(c_VkHardware.m_VmaAllocator, stagingBuffer.m_Allocation, &meshData));
	memcpy(meshData, m_Mesh.m_Vertices.data(), vertexBufferSize);
	vmaUnmapMemory(c_VkHardware.m_VmaAllocator, stagingBuffer.m_Allocation);

	// Create GPU side buffer
	c_VkHardware.CreateVMABuffer(m_VertexBuffer,
		vertexBufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY);

	c_VkRenderer.SubmitToRenderer([=, this](VkCommandBuffer commandBuffer) 
	{
		VkBufferCopy bufferCopy = VkBufferCopy();
		bufferCopy.dstOffset = 0;
		bufferCopy.srcOffset = 0;
		bufferCopy.size = vertexBufferSize;

		vkCmdCopyBuffer(commandBuffer, stagingBuffer.m_Buffer, m_VertexBuffer.m_Buffer, 1, &bufferCopy);
	});

	// Destroy staging buffer
	vmaDestroyBuffer(c_VkHardware.m_VmaAllocator, stagingBuffer.m_Buffer, stagingBuffer.m_Allocation);
}

void vkn::VkModel::AllocateIndexBuffer()
{
	const size_t indexBufferSize = sizeof(m_Mesh.m_Indices[0]) * static_cast<uint32_t>(m_Mesh.m_Indices.size());
	if (indexBufferSize == 0)
		return;

	VmaAllocatedBuffer stagingBuffer;

	// Create CPU side staging buffer
	c_VkHardware.CreateVMABuffer(stagingBuffer,
		indexBufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_ONLY);

	void* meshData;
	VK_CALL(vmaMapMemory(c_VkHardware.m_VmaAllocator, stagingBuffer.m_Allocation, &meshData));
	memcpy(meshData, m_Mesh.m_Indices.data(), indexBufferSize);
	vmaUnmapMemory(c_VkHardware.m_VmaAllocator, stagingBuffer.m_Allocation);

	// Create GPU side buffer
	c_VkHardware.CreateVMABuffer(m_IndexBuffer,
		indexBufferSize,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY);

	c_VkRenderer.SubmitToRenderer([=, this](VkCommandBuffer commandBuffer) 
	{
		VkBufferCopy bufferCopy;
		bufferCopy.dstOffset = 0;
		bufferCopy.srcOffset = 0;
		bufferCopy.size = indexBufferSize;

		vkCmdCopyBuffer(commandBuffer, stagingBuffer.m_Buffer, m_IndexBuffer.m_Buffer, 1, &bufferCopy);
	});

	// Destroy staging buffer
	vmaDestroyBuffer(c_VkHardware.m_VmaAllocator, stagingBuffer.m_Buffer, stagingBuffer.m_Allocation);
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
