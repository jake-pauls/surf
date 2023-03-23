#include "VkModel.h"

#include "VkMesh.h"
#include "VkTypes.h"
#include "VkRenderer.h"
#include "VkHardware.h"
#include "VkInitializers.h"

vkn::VkModel::VkModel(const VkRenderer& renderer, const VkMesh& mesh, VkMaterial* material)
	: c_VkRenderer(renderer)
	, c_VkHardware(*renderer.m_VkHardware)
	, m_Mesh(mesh)
	, m_Material(material)
	, m_ModelMatrix(glm::mat4(1.0f))
{
	AllocateVertexBuffer();
	AllocateIndexBuffer();
	AllocateDescriptorSets();
}

vkn::VkModel::~VkModel()
{
	core::Log(ELogType::Trace, "[VkModel] Destroying a model's vertex, index, and uniform buffers");

	vmaDestroyBuffer(c_VkHardware.m_VmaAllocator, m_VertexBuffer.m_Buffer, m_VertexBuffer.m_Allocation);
	if (HasIndexBuffer())
		vmaDestroyBuffer(c_VkHardware.m_VmaAllocator, m_IndexBuffer.m_Buffer, m_IndexBuffer.m_Allocation);

	// Destroy model uniform buffers
	for (size_t i = 0; i < c_VkRenderer.c_MaxFramesInFlight; ++i)
	{
		vmaDestroyBuffer(c_VkHardware.m_VmaAllocator, m_UniformBuffers[i].m_Buffer, m_UniformBuffers[i].m_Allocation);
	}
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

void vkn::VkModel::AllocateDescriptorSets()
{
	//! UBO
	//! All mesh shaders have UBOs at the moment
	
	const size_t uniformBufferSize = sizeof(VkMeshUniformBufferObject);
	const VkShaderPipeline* modelPipeline = m_Material->GetShaderPipelinePtr();

	m_UniformBuffers.resize(c_VkRenderer.c_MaxFramesInFlight);
	for (size_t i = 0; i < c_VkRenderer.c_MaxFramesInFlight; ++i)
	{
		c_VkHardware.CreateVMABuffer(m_UniformBuffers[i],
			uniformBufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VMA_MEMORY_USAGE_CPU_TO_GPU);

		VkDescriptorSetAllocateInfo descriptorSetAllocInfo = vkn::InitDescriptorSetAllocateInfo(modelPipeline->m_DescriptorPool, &modelPipeline->m_DescriptorSetLayout);

		VK_CALL(vkAllocateDescriptorSets(c_VkHardware.m_LogicalDevice, &descriptorSetAllocInfo, &m_UniformBuffers[i].m_Descriptor));

		VkDescriptorBufferInfo descriptorBufferInfo = vkn::InitDescriptorBufferInfo(m_UniformBuffers[i].m_Buffer, sizeof(VkMeshUniformBufferObject));
		VkWriteDescriptorSet descriptorSetWrite = vkn::InitWriteDescriptorSetBuffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, m_UniformBuffers[i].m_Descriptor, &descriptorBufferInfo, 0);
		vkUpdateDescriptorSets(c_VkHardware.m_LogicalDevice, 1, &descriptorSetWrite, 0, nullptr);
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
