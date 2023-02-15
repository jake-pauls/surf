#include "VkMesh.h"

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