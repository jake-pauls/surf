#include "VkMesh.h"

#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

vkn::VkVertexInputDescription vkn::VkVertex::GetVertexInputDescription()
{
	VkVertexInputDescription vertexInputDescription = VkVertexInputDescription();

	// Input binding for a single vertex
	VkVertexInputBindingDescription mainBinding = VkVertexInputBindingDescription();
	mainBinding.binding = 0;
	mainBinding.stride = sizeof(VkVertex);
	mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	vertexInputDescription.m_Bindings.push_back(mainBinding);

	// Position is stored in location 0
	VkVertexInputAttributeDescription positionAttribute = VkVertexInputAttributeDescription();
	positionAttribute.binding = 0;
	positionAttribute.location = 0;
	positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	positionAttribute.offset = offsetof(VkVertex, m_Position);

	// Normal is stored in location 1 
	VkVertexInputAttributeDescription normalAttribute = VkVertexInputAttributeDescription();
	normalAttribute.binding = 0;
	normalAttribute.location = 1;
	normalAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	normalAttribute.offset = offsetof(VkVertex, m_Normal);

	// Color is stored in location 2
	VkVertexInputAttributeDescription colorAttribute = VkVertexInputAttributeDescription();
	colorAttribute.binding = 0;
	colorAttribute.location = 2;
	colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	colorAttribute.offset = offsetof(VkVertex, m_Color);


	// UV is stored in location 3
	VkVertexInputAttributeDescription uvAttribute = VkVertexInputAttributeDescription();
	uvAttribute.binding = 0;
	uvAttribute.location = 3;
	uvAttribute.format = VK_FORMAT_R32G32_SFLOAT;
	uvAttribute.offset = offsetof(VkVertex, m_UV);

	// Join attributes
	vertexInputDescription.m_Attributes.push_back(positionAttribute);
	vertexInputDescription.m_Attributes.push_back(normalAttribute);
	vertexInputDescription.m_Attributes.push_back(colorAttribute);
	vertexInputDescription.m_Attributes.push_back(uvAttribute);

	return vertexInputDescription;
}

void vkn::VkMesh::LoadFromObj(const char* filename)
{
	tinyobj::ObjReader reader;
	tinyobj::ObjReaderConfig readerConfig;
	readerConfig.mtl_search_path = core::FileSystem::GetAssetsDirectory().string();
	readerConfig.triangulate = true;

	if (!reader.ParseFromFile(filename, readerConfig))
	{
		if (!reader.Error().empty())
			core::Log(ELogType::Error, "[tinyobj] {}", reader.Error());

		return;
	}

	if (!reader.Warning().empty())
		core::Log(ELogType::Warn, "[tinyobj] {}", reader.Warning());

	m_Vertices.clear();
	m_Indices.clear();

	tinyobj::attrib_t attrib = reader.GetAttrib();
	std::vector<tinyobj::shape_t> shapes = reader.GetShapes();
	std::vector<tinyobj::material_t> materials = reader.GetMaterials();

	std::unordered_map<VkVertex, uint32_t> uniqueVertices{};
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			VkVertex vertex = VkVertex();

			if (index.vertex_index >= 0)
			{
				vertex.m_Position = { 
					attrib.vertices[3 * index.vertex_index + 0], 
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};
			}

			if (index.normal_index >= 0)
			{
				vertex.m_Normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};
			}

			if (index.texcoord_index >= 0)
			{
				vertex.m_UV = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					(1 - attrib.texcoords[2 * index.texcoord_index + 1])
				};
			}

			// Testing: Use the normals for color
			vertex.m_Color = vertex.m_Normal;

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(m_Vertices.size());
				m_Vertices.push_back(vertex);
			}

			m_Indices.push_back(uniqueVertices[vertex]);
		}
	}
}
