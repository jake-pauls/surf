#include "VkRenderer.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include <glm/gtx/transform.hpp>

#if defined(__linux__)
#define STBI_NO_SIMD
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "VkPass.h"
#include "VkModel.h"
#include "VkHardware.h"
#include "VkRendererContext.h"
#include "VkShaderPipeline.h"
#include "VkInitializers.h"

vkn::VkRenderer::VkRenderer(wv::Window* window)
	: m_Window(window)
{
}

void vkn::VkRenderer::Init()
{
	core::Log(ELogType::Trace, "[VkRenderer] Initializing Vulkan renderer");

	// Default hardware
	{
		m_VkHardware = new VkHardware(m_Window);
	}

	// Swap chain and render passes
	{
		m_VkSwapChain = new VkSwapChain(*this, *m_VkHardware, m_Window);
		m_DefaultPass = new VkPass(m_VkHardware->m_LogicalDevice, *m_VkSwapChain);

		// Framebuffers (need to be initialized after initial renderpass)
		m_VkSwapChain->CreateFramebuffers();
	}

	// Shader generation
	{
		std::string triangleVertexShader = core::FileSystem::GetShaderPath("TriangleMesh.vert.hlsl.spv").string();
		std::string triangleFragmentShader = core::FileSystem::GetShaderPath("TriangleMesh.frag.hlsl.spv").string();
		m_DefaultPipeline = new VkShaderPipeline(*this, *m_VkHardware, triangleVertexShader, triangleFragmentShader);
	}

	// Create command pools and sync objects
	CreateCommands();
	CreateSyncObjects();

	// Load textures
	{
		std::string texturePath = core::FileSystem::GetAssetPath("viking_room.png").string();
		CreateTexture(texturePath);
		VkImageViewCreateInfo textureInfo = vkn::InitImageViewCreateInfo(VK_FORMAT_R8G8B8A8_SRGB, m_Texture.m_Image.m_Image, VK_IMAGE_ASPECT_COLOR_BIT);
		vkCreateImageView(m_VkHardware->m_LogicalDevice, &textureInfo, nullptr, &m_Texture.m_Image.m_ImageView);
	}

	// Load 3D meshes
	LoadMeshes();

	CreateUniformBuffers();
}

void vkn::VkRenderer::Draw()
{
	VkCommandBuffer commandBuffer = BeginFrame();
	if (commandBuffer == VK_NULL_HANDLE) 
		return;

	BeginRenderPass(commandBuffer);

	DrawCommandBuffer(commandBuffer);

	EndRenderPass(commandBuffer);

	EndFrame();
}

void vkn::VkRenderer::Teardown()
{
	core::Log(ELogType::Trace, "[VkRenderer] Tearing down Vulkan renderer");

	// Wait for logical device to finish operations before tearing down
	VK_CALL(vkDeviceWaitIdle(m_VkHardware->m_LogicalDevice));

	delete m_LoadedModel;

	// TODO: Implement main destruction queue as opposed to relying on destructors
	delete m_VkSwapChain;
	delete m_DefaultPipeline;
	delete m_DefaultPass;

	// TODO: Abstract uniform buffers somewhere?
	for (size_t i = 0; i < c_MaxFramesInFlight; ++i)
	{
		vmaDestroyBuffer(m_VkHardware->m_VmaAllocator, m_UniformBuffers[i].m_Buffer, m_UniformBuffers[i].m_Allocation);
	}

	// TODO: Abstract texture loading/destruction
	vkDestroySampler(m_VkHardware->m_LogicalDevice, m_Texture.m_Sampler, nullptr);
	vkDestroyImageView(m_VkHardware->m_LogicalDevice, m_Texture.m_Image.m_ImageView, nullptr);
	vmaDestroyImage(m_VkHardware->m_VmaAllocator, m_Texture.m_Image.m_Image, m_Texture.m_Image.m_Allocation);

	// Sync objects
	for (size_t i = 0; i < c_MaxFramesInFlight; ++i)
	{
		vkDestroySemaphore(m_VkHardware->m_LogicalDevice, m_ImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(m_VkHardware->m_LogicalDevice, m_RenderFinishedSemaphores[i], nullptr);
		vkDestroyFence(m_VkHardware->m_LogicalDevice, m_InFlightFences[i], nullptr);
	}

	// Upload context
	vkDestroyFence(m_VkHardware->m_LogicalDevice, m_UploadContext.m_UploadFence, nullptr);
	vkFreeCommandBuffers(m_VkHardware->m_LogicalDevice, m_UploadContext.m_CommandPool, 1, &m_UploadContext.m_CommandBuffer);
	vkDestroyCommandPool(m_VkHardware->m_LogicalDevice, m_UploadContext.m_CommandPool, nullptr);

	// Command buffers/pools
	vkFreeCommandBuffers(m_VkHardware->m_LogicalDevice, m_CommandPool, static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
	m_CommandBuffers.clear();
	vkDestroyCommandPool(m_VkHardware->m_LogicalDevice, m_CommandPool, nullptr);

	// Hardware
	delete m_VkHardware;
}

void vkn::VkRenderer::CreateSyncObjects()
{
	//! Runtime

	m_ImageAvailableSemaphores.resize(c_MaxFramesInFlight);
	m_RenderFinishedSemaphores.resize(c_MaxFramesInFlight);
	m_InFlightFences.resize(c_MaxFramesInFlight);

	VkSemaphoreCreateInfo semaphoreCreateInfo = vkn::InitSemaphoreCreateInfo();
	VkFenceCreateInfo fenceCreateInfo = vkn::InitFenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);

	for (size_t i = 0; i < c_MaxFramesInFlight; ++i)
	{
		VK_CALL(vkCreateSemaphore(m_VkHardware->m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphores[i]));
		VK_CALL(vkCreateSemaphore(m_VkHardware->m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_RenderFinishedSemaphores[i]));
		VK_CALL(vkCreateFence(m_VkHardware->m_LogicalDevice, &fenceCreateInfo, nullptr, &m_InFlightFences[i]));
	}

	//! Upload

	VkFenceCreateInfo uploadFenceCreateInfo = vkn::InitFenceCreateInfo();
	VK_CALL(vkCreateFence(m_VkHardware->m_LogicalDevice, &uploadFenceCreateInfo, nullptr, &m_UploadContext.m_UploadFence));
}

void vkn::VkRenderer::CreateCommands()
{
	//! Runtime

	QueueFamily queueFamily = m_VkHardware->FindQueueFamilies(m_VkHardware->m_PhysicalDevice);
	VkCommandPoolCreateInfo commandPoolInfo = vkn::InitCommandPoolCreateInfo(queueFamily.m_GraphicsFamily.value(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	VK_CALL(vkCreateCommandPool(m_VkHardware->m_LogicalDevice, &commandPoolInfo, nullptr, &m_CommandPool));

	m_CommandBuffers.resize(c_MaxFramesInFlight);
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = vkn::InitCommandBufferAllocateInfo(m_CommandPool, 2);
	VK_CALL(vkAllocateCommandBuffers(m_VkHardware->m_LogicalDevice, &commandBufferAllocateInfo, m_CommandBuffers.data()));

	//! Upload

	VkCommandPoolCreateInfo uploadCommandPoolInfo = vkn::InitCommandPoolCreateInfo(queueFamily.m_GraphicsFamily.value());
	VK_CALL(vkCreateCommandPool(m_VkHardware->m_LogicalDevice, &uploadCommandPoolInfo, nullptr, &m_UploadContext.m_CommandPool));

	VkCommandBufferAllocateInfo uploadCommandBufferAllocateInfo = vkn::InitCommandBufferAllocateInfo(m_UploadContext.m_CommandPool);
	VK_CALL(vkAllocateCommandBuffers(m_VkHardware->m_LogicalDevice, &uploadCommandBufferAllocateInfo, &m_UploadContext.m_CommandBuffer));
}

void vkn::VkRenderer::CreateUniformBuffers()
{
	//! UBO

	const size_t uniformBufferSize = sizeof(VkMeshUniformBufferObject);

	m_UniformBuffers.resize(c_MaxFramesInFlight);
	for (size_t i = 0; i < c_MaxFramesInFlight; ++i)
	{
		m_VkHardware->CreateVMABuffer(m_UniformBuffers[i],
			uniformBufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VMA_MEMORY_USAGE_CPU_TO_GPU);

		VkDescriptorSetAllocateInfo descriptorSetAllocInfo = vkn::InitDescriptorSetAllocateInfo(m_DefaultPipeline->m_DescriptorPool, &m_DefaultPipeline->m_DescriptorSetLayout);

		VK_CALL(vkAllocateDescriptorSets(m_VkHardware->m_LogicalDevice, &descriptorSetAllocInfo, &m_UniformBuffers[i].m_Descriptor));

		VkDescriptorBufferInfo descriptorBufferInfo = vkn::InitDescriptorBufferInfo(m_UniformBuffers[i].m_Buffer, sizeof(VkMeshUniformBufferObject));
		VkWriteDescriptorSet descriptorSetWrite = vkn::InitWriteDescriptorSetBuffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, m_UniformBuffers[i].m_Descriptor, &descriptorBufferInfo, 0);
		vkUpdateDescriptorSets(m_VkHardware->m_LogicalDevice, 1, &descriptorSetWrite, 0, nullptr);
	}

	//! Sampler

	m_Texture.m_Sampler = VkSampler();
	VkSamplerCreateInfo samplerCreateInfo = vkn::InitSamplerCreateInfo(VK_FILTER_NEAREST);
	vkCreateSampler(m_VkHardware->m_LogicalDevice, &samplerCreateInfo, nullptr, &m_Texture.m_Sampler);

	VkDescriptorSetAllocateInfo descriptorSetAllocInfo = vkn::InitDescriptorSetAllocateInfo(m_DefaultPipeline->m_DescriptorPool, &m_DefaultPipeline->m_SingleTextureSetLayout);
	VK_CALL(vkAllocateDescriptorSets(m_VkHardware->m_LogicalDevice, &descriptorSetAllocInfo, &m_Texture.m_Image.m_Descriptor));

	VkDescriptorImageInfo textureBufferInfo = VkDescriptorImageInfo();
	textureBufferInfo.sampler = m_Texture.m_Sampler;
	textureBufferInfo.imageView = m_Texture.m_Image.m_ImageView;
	textureBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	
	VkWriteDescriptorSet descriptorSetWrite = vkn::InitWriteDescriptorSetImage(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, m_Texture.m_Image.m_Descriptor, &textureBufferInfo, 0);
	vkUpdateDescriptorSets(m_VkHardware->m_LogicalDevice, 1, &descriptorSetWrite, 0, nullptr);
}

void vkn::VkRenderer::SubmitToRenderer(std::function<void(VkCommandBuffer)>&& submitFunction) const
{
	VkCommandBuffer commandBuffer = m_UploadContext.m_CommandBuffer;

	VkCommandBufferBeginInfo commandBufferBeginInfo = vkn::InitCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	VK_CALL(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

	// Execute submitted function
	submitFunction(commandBuffer);

	VK_CALL(vkEndCommandBuffer(commandBuffer));

	// Submit command buffer to the queue and execute it
	// The upload context's fence will block until graphics commands finish executing
	VkSubmitInfo submitInfo = vkn::InitSubmitInfo(&commandBuffer);
	VK_CALL(vkQueueSubmit(m_VkHardware->m_GraphicsQueue, 1, &submitInfo, m_UploadContext.m_UploadFence));
	
	VK_CALL(vkWaitForFences(m_VkHardware->m_LogicalDevice, 1, &m_UploadContext.m_UploadFence, true, UINT64_MAX));
	VK_CALL(vkResetFences(m_VkHardware->m_LogicalDevice, 1, &m_UploadContext.m_UploadFence));
	VK_CALL(vkResetCommandPool(m_VkHardware->m_LogicalDevice, m_UploadContext.m_CommandPool, 0));
}

VkCommandBuffer vkn::VkRenderer::BeginFrame()
{
	WAVE_ASSERT(!m_IsFrameStarted, "Cannot call BeginFrame while another frame is in progress");

	VkResult result = m_VkSwapChain->AcquireNextImage(&m_CurrentImageIndex);

	WAVE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR, "Failed to acquire next image from the swap chain");
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		m_VkSwapChain->RecreateSwapchain();
		return VK_NULL_HANDLE;
	}

	VK_CALL(vkResetFences(m_VkHardware->m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrameIndex]));

	m_IsFrameStarted = true;

	VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
	VK_CALL(vkResetCommandBuffer(commandBuffer, 0));

	VkCommandBufferBeginInfo commandBufferInfo = vkn::InitCommandBufferBeginInfo();
	VK_CALL(vkBeginCommandBuffer(commandBuffer, &commandBufferInfo));

	return commandBuffer;
}

void vkn::VkRenderer::EndFrame()
{
	WAVE_ASSERT(m_IsFrameStarted, "Cannot call BeginFrame without a frame in progress");

	VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
	VK_CALL(vkEndCommandBuffer(commandBuffer));

	VkResult result = m_VkSwapChain->SubmitCommandBuffers(&commandBuffer, m_CurrentImageIndex);
	WAVE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR, "Failed to present swap chain image");

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		m_VkSwapChain->RecreateSwapchain();

	m_IsFrameStarted = false;
	m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % c_MaxFramesInFlight;
}

void vkn::VkRenderer::BeginRenderPass(VkCommandBuffer commandBuffer)
{
	WAVE_ASSERT(m_IsFrameStarted, "Cannot begin render pass without a frame in progress");
	WAVE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Cannot end render pass on a command buffer from a different frame");

	const VkExtent2D& swapChainExtent = m_VkSwapChain->m_SwapChainExtent;
	VkRenderPassBeginInfo renderPassInfo = vkn::InitRenderPassBeginInfo(m_DefaultPass->m_RenderPass, m_VkSwapChain->m_VkSwapChainFramebuffers[m_CurrentImageIndex], swapChainExtent);

	VkClearValue clearColor;
	clearColor.color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
	VkClearValue depthClear;
	depthClear.depthStencil.depth = 1.0f;

	VkClearValue clearValues[] = { clearColor, depthClear };
	renderPassInfo.pClearValues = &clearValues[0];
	renderPassInfo.clearValueCount = 2;

	// Start render pass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Setup viewport/scissor
	VkViewport viewport = VkViewport();
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = VkRect2D();
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void vkn::VkRenderer::DrawCommandBuffer(VkCommandBuffer commandBuffer)
{
	const VkExtent2D extent = m_VkSwapChain->m_SwapChainExtent;

	// MVP/Uniform Testing
	glm::vec3 cameraPosition = { 0.0f, 0.0f, -2.0f };
	glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), cameraPosition);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(extent.width) / static_cast<float>(extent.height), 0.1f, 200.0f);
	projectionMatrix[1][1] *= -1;

	glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(core::Timer::DeltaTime() * 0.02f), glm::vec3(1.0f, 0.0f, 0.0f));

	// Bind required pipeline
	m_DefaultPipeline->Bind(commandBuffer);

	// Bind geometry
	m_LoadedModel->Bind(commandBuffer);

	// Bind texture descriptor set
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_DefaultPipeline->m_PipelineLayout, 1, 1, &m_Texture.m_Image.m_Descriptor, 0, nullptr);

	// Submit push constants (constant uniforms)
	VkMeshPushConstants constants;
	constants.m_ModelMatrix = modelMatrix;
	vkCmdPushConstants(commandBuffer, m_DefaultPipeline->m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(VkMeshPushConstants), &constants);

	// Submit uniforms through descriptor sets
	VkMeshUniformBufferObject uniform;
	uniform.m_ProjectionMatrix = projectionMatrix;
	uniform.m_ViewMatrix = viewMatrix;

	void* data;
	VK_CALL(vmaMapMemory(m_VkHardware->m_VmaAllocator, m_UniformBuffers[m_CurrentFrameIndex].m_Allocation, &data));
	memcpy(data, &uniform, sizeof(VkMeshUniformBufferObject));
	vmaUnmapMemory(m_VkHardware->m_VmaAllocator, m_UniformBuffers[m_CurrentFrameIndex].m_Allocation);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_DefaultPipeline->m_PipelineLayout, 0, 1, &m_UniformBuffers[m_CurrentFrameIndex].m_Descriptor, 0, nullptr);

	// Draw geometry
	m_LoadedModel->Draw(commandBuffer);
}

void vkn::VkRenderer::EndRenderPass(VkCommandBuffer commandBuffer)
{
	WAVE_ASSERT(m_IsFrameStarted, "Cannot end render pass without a frame in progress");
	WAVE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Cannot render pass on a command buffer from a different frame");

	vkCmdEndRenderPass(commandBuffer);
}

void vkn::VkRenderer::LoadMeshes()
{
	m_LoadedMesh.LoadFromObj(core::FileSystem::GetAssetPath("viking_room.obj").string().c_str());
	m_LoadedModel = new VkModel(*this, m_LoadedMesh);
}

void vkn::VkRenderer::CreateTexture(const std::string& filename)
{
	int texWidth, texHeight, channels;
	stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &channels, STBI_rgb_alpha);
	WAVE_ASSERT(pixels, "Failed to load texture from provided image file");

	VkDeviceSize imageSize = texWidth * texHeight * 4;

	VmaAllocatedBuffer stagingBuffer;
	m_VkHardware->CreateVMABuffer(stagingBuffer,
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_ONLY);

	void* texData;
	VK_CALL(vmaMapMemory(m_VkHardware->m_VmaAllocator, stagingBuffer.m_Allocation, &texData));
	memcpy(texData, pixels, static_cast<size_t>(imageSize));
	vmaUnmapMemory(m_VkHardware->m_VmaAllocator, stagingBuffer.m_Allocation);

	stbi_image_free(pixels);

	/// ----------------

	VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB;

	VkExtent3D imageExtent;
	imageExtent.width = static_cast<uint32_t>(texWidth);
	imageExtent.height = static_cast<uint32_t>(texHeight);
	imageExtent.depth = 1;
	
	VkImageCreateInfo newImageCreateInfo = vkn::InitImageCreateInfo(imageFormat, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, imageExtent);
	newImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

	VmaAllocationCreateInfo newImageAllocationCreateInfo = VmaAllocationCreateInfo();
	newImageAllocationCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	// Allocate and create image
	vmaCreateImage(m_VkHardware->m_VmaAllocator, &newImageCreateInfo, &newImageAllocationCreateInfo, &m_Texture.m_Image.m_Image, &m_Texture.m_Image.m_Allocation, nullptr);

	/// ----------------

	// Submit command to render image
	SubmitToRenderer([=](VkCommandBuffer commandBuffer) 
	{
		VkImageSubresourceRange range;
		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseMipLevel = 0;
		range.levelCount = 1;
		range.baseArrayLayer = 0;
		range.layerCount = 1;

		VkImageMemoryBarrier transferImageBarrier = VkImageMemoryBarrier();
		transferImageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		transferImageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		transferImageBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		transferImageBarrier.image = m_Texture.m_Image.m_Image;
		transferImageBarrier.subresourceRange = range;
		transferImageBarrier.srcAccessMask = 0;
		transferImageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		// Barrier the image into the transfer-receive layout
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &transferImageBarrier);

		VkBufferImageCopy copyRegion = VkBufferImageCopy();
		copyRegion.bufferOffset = 0;
		copyRegion.bufferRowLength = 0;
		copyRegion.bufferImageHeight = 0;
		copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyRegion.imageSubresource.mipLevel = 0;
		copyRegion.imageSubresource.baseArrayLayer = 0;
		copyRegion.imageSubresource.layerCount = 1;
		copyRegion.imageExtent = imageExtent;

		// Copy the image into a staging buffer
		vkCmdCopyBufferToImage(commandBuffer, stagingBuffer.m_Buffer, m_Texture.m_Image.m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

		VkImageMemoryBarrier readableImageBarrier = transferImageBarrier;
		readableImageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		readableImageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		readableImageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		readableImageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		// Barrier the image into the shader layout
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &readableImageBarrier);
	});

	/// ----------------

	// Cleanup local resources
	vmaDestroyBuffer(m_VkHardware->m_VmaAllocator, stagingBuffer.m_Buffer, stagingBuffer.m_Allocation);
}
