#include "VkMesh.h"

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

	// Join attributes
	vertexInputDescription.m_Attributes.push_back(positionAttribute);
	vertexInputDescription.m_Attributes.push_back(normalAttribute);
	vertexInputDescription.m_Attributes.push_back(colorAttribute);

	return vertexInputDescription;
}

bool vkn::VkMesh::LoadFromObj(const char* filename)
{
	tinyobj::ObjReader reader;
	tinyobj::ObjReaderConfig readerConfig;
	readerConfig.mtl_search_path = core::FileSystem::GetAssetsDirectory().string();

	if (!reader.ParseFromFile(filename, readerConfig))
	{
		if (!reader.Error().empty())
			core::Log(ELogType::Error, "[tinyobj] {}", reader.Error());

		return false;
	}

	if (!reader.Warning().empty())
		core::Log(ELogType::Warn, "[tinyobj] {}", reader.Warning());


	tinyobj::attrib_t attrib = reader.GetAttrib();
	std::vector<tinyobj::shape_t> shapes = reader.GetShapes();
	std::vector<tinyobj::material_t> materials = reader.GetMaterials();

	// Loop over each shape
	for (size_t si = 0; si < shapes.size(); ++si)
	{
		// Loop over each face/polygon
		size_t indexOffset = 0;
		for (size_t fi = 0; fi < shapes[si].mesh.num_face_vertices.size(); ++fi)
		{
			// Hardcoded: Loading triangles
			int faceVertexCount = shapes[si].mesh.num_face_vertices[fi];
			
			// Loop over each vertex in the face 
			for (size_t vi = 0; vi < faceVertexCount; ++vi)
			{
				VkVertex vertex = VkVertex();
				tinyobj::index_t idx = shapes[si].mesh.indices[indexOffset + vi];

				vertex.m_Position = { 
					attrib.vertices[3 * idx.vertex_index + 0], 
					attrib.vertices[3 * idx.vertex_index + 1],
					attrib.vertices[3 * idx.vertex_index + 2]
				};

				if (idx.normal_index >= 0)
				{
					vertex.m_Normal = {
						attrib.normals[3 * idx.normal_index + 0],
						attrib.normals[3 * idx.normal_index + 1],
						attrib.normals[3 * idx.normal_index + 2],
					};
				}

				if (idx.texcoord_index >= 0)
				{
					// TODO: Add texture coordinates to the vertex definition
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				}

				// Testing: Use the normals for color
				vertex.m_Color = vertex.m_Normal; 

				m_Vertices.push_back(vertex);
			}

			indexOffset += faceVertexCount;
		}
	}

	return true;
}
