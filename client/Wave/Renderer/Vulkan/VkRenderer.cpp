#include "VkRenderer.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include <glm/gtx/transform.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>

#include "Camera.h"
#include "ToolPanel.h"
#include "SurfEngine.h"
#include "SurfStore.h"

#include "VkPass.h"
#include "VkModel.h"
#include "VkHardware.h"
#include "VkRendererContext.h"
#include "VkShaderPipeline.h"
#include "VkInitializers.h"

void surf_SampleCFunction(surf_argpack_t argpack) 
{ 
	core::Log(ELogType::Debug, "callback: surf_SampleCFunction");
}

void surf_SampleCFunctionWithArgs(surf_argpack_t argpack) 
{ 
	int aInt = surf_ArgpackGetInt(argpack, 0);
	float aFlt = surf_ArgpackGetFlt(argpack, 1);

	// TODO: Strings can only be placed as the last arg of an argpack 
	//		 Presumably for alignment reasons?
	char* aStr = surf_ArgpackGetStr(argpack, 2);

	core::Log(ELogType::Debug, "callback: surf_SampleCFunctionWithArgs {} {} {}", aInt, aFlt, aStr);
}

///  
///
///

vkn::VkRenderer::VkRenderer(wv::Window* window, wv::Camera* camera)
	: m_Window(window)
	, m_Camera(camera)
{
}

void vkn::VkRenderer::Init()
{
	core::Log(ELogType::Trace, "[VkRenderer] Initializing Vulkan renderer");

	RegisterSurfSymbols();

	// Default hardware
	m_VkHardware = new VkHardware(m_Window);

	// Swap chain and render passes
	m_VkSwapChain = new VkSwapChain(*this, *m_VkHardware, m_Window);
	m_DefaultPass = new VkPass(m_VkHardware->m_LogicalDevice, *m_VkSwapChain);

	// Framebuffers (need to be initialized after initial renderpass)
	m_VkSwapChain->CreateFramebuffers();

	// Create command pools and sync objects
	CreateCommands();
	CreateSyncObjects();

	// Shader creation 
	CreatePipelines();

	LoadMeshes();

	InitImGui();
}

void vkn::VkRenderer::Draw()
{
	VkCommandBuffer commandBuffer = BeginFrame();
	if (commandBuffer == VK_NULL_HANDLE) 
		return;

	// Draw ImGui
	ImGui::Render();

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

	// TODO: Implement main destruction queue as opposed to relying on destructors
	delete m_LoadedModel;
	delete m_VkSwapChain;

	delete m_UntexturedPipeline;
	delete m_TexturedPipeline;
	delete m_PBRPipeline;
	delete m_RustPBRPipeline;
	delete m_BambooPBRPipeline;
	delete m_SandPBRPipeline;
	delete m_RockPBRPipeline;

	delete m_DefaultPass;

	// Destroy materials
	m_Materials.clear();
	m_RenderableModels.clear();

	// Destroy ImGui
	vkDestroyDescriptorPool(m_VkHardware->m_LogicalDevice, m_ImguiDescriptorPool, nullptr);
	ImGui_ImplVulkan_Shutdown();

	// Destroy sync objects
	for (size_t i = 0; i < c_MaxFramesInFlight; ++i)
	{
		vkDestroySemaphore(m_VkHardware->m_LogicalDevice, m_ImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(m_VkHardware->m_LogicalDevice, m_RenderFinishedSemaphores[i], nullptr);
		vkDestroyFence(m_VkHardware->m_LogicalDevice, m_InFlightFences[i], nullptr);
	}

	// Destroy upload context
	vkDestroyFence(m_VkHardware->m_LogicalDevice, m_UploadContext.m_UploadFence, nullptr);
	vkFreeCommandBuffers(m_VkHardware->m_LogicalDevice, m_UploadContext.m_CommandPool, 1, &m_UploadContext.m_CommandBuffer);
	vkDestroyCommandPool(m_VkHardware->m_LogicalDevice, m_UploadContext.m_CommandPool, nullptr);

	// Destroy command buffers/pools
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

void vkn::VkRenderer::CreatePipelines()
{
	core::Log(ELogType::Info, "[VkRenderer] Loading shaders... this might take a while");

	std::string untexturedVertexShader = core::FileSystem::GetShaderPath("UntexturedMesh.vert.hlsl.spv").string();
	std::string untexturedFragmentShader = core::FileSystem::GetShaderPath("UntexturedMesh.frag.hlsl.spv").string();

	std::string texturedVertexShader = core::FileSystem::GetShaderPath("TexturedMesh.vert.hlsl.spv").string();
	std::string texturedFragmentShader = core::FileSystem::GetShaderPath("TexturedMesh.frag.hlsl.spv").string();

	std::string uPBRVertexShader = core::FileSystem::GetShaderPath("UntexturedPBR.vert.glsl.spv").string();
	std::string uPBRFragmentShader = core::FileSystem::GetShaderPath("UntexturedPBR.frag.glsl.spv").string();

	std::string tPBRVertexShader = core::FileSystem::GetShaderPath("TexturedPBR.vert.glsl.spv").string();
	std::string tPBRFragmentShader = core::FileSystem::GetShaderPath("TexturedPBR.frag.glsl.spv").string();

	m_UntexturedPipeline = new VkShaderPipeline(*this, *m_VkHardware, untexturedVertexShader, untexturedFragmentShader);
	m_TexturedPipeline = new VkShaderPipeline(*this, *m_VkHardware, texturedVertexShader, texturedFragmentShader, 1);
	m_PBRPipeline = new VkShaderPipeline(*this, *m_VkHardware, uPBRVertexShader, uPBRFragmentShader);
	m_RustPBRPipeline = new VkShaderPipeline(*this, *m_VkHardware, tPBRVertexShader, tPBRFragmentShader, 5);
	m_BambooPBRPipeline = new VkShaderPipeline(*this, *m_VkHardware, tPBRVertexShader, tPBRFragmentShader, 5);
	m_SandPBRPipeline = new VkShaderPipeline(*this, *m_VkHardware, tPBRVertexShader, tPBRFragmentShader, 5);
	m_RockPBRPipeline = new VkShaderPipeline(*this, *m_VkHardware, tPBRVertexShader, tPBRFragmentShader, 5);

	CreateMaterial(*m_UntexturedPipeline, "Default");
	CreateTexturedMaterial(*m_TexturedPipeline, { "viking_room.png" }, "Viking Room");
	CreateMaterial(*m_PBRPipeline, "PBR Default");

	std::vector<std::string> rustedIronTextures = {
		"PBR/Rust/Rust_Albedo.png",
		"PBR/Rust/Rust_Normal.png",
		"PBR/Rust/Rust_Metallic.png",
		"PBR/Rust/Rust_Roughness.png",
		// "PBR/Rust/Rust_AO.png",
		"PBR/Rust/Rust_Albedo.png",
	};

	CreateTexturedMaterial(*m_RustPBRPipeline, rustedIronTextures, "Rust (PBR)");

	std::vector<std::string> bambooTextures = {
		"PBR/Bamboo/Bamboo_Albedo.png",
		"PBR/Bamboo/Bamboo_Normal.png",
		"PBR/Bamboo/Bamboo_Metallic.png",
		"PBR/Bamboo/Bamboo_Roughness.png",
		//"PBR/Bamboo/Bamboo_AO.png"
		"PBR/Bamboo/Bamboo_Albedo.png"
	};

	CreateTexturedMaterial(*m_BambooPBRPipeline, bambooTextures, "Bamboo (PBR)");

	std::vector<std::string> sandTextures = {
		"PBR/Sand/Sand_Albedo.png",
		"PBR/Sand/Sand_Normal.png",
		"PBR/Sand/Sand_Metallic.png",
		"PBR/Sand/Sand_Roughness.png",
		//"PBR/Sand/Sand_AO.png"
		"PBR/Sand/Sand_Albedo.png"
	};

	CreateTexturedMaterial(*m_SandPBRPipeline, sandTextures, "Sand (PBR)");

	std::vector<std::string> rockTextures = {
		"PBR/Rock/Rock_Albedo.png",
		"PBR/Rock/Rock_Normal.png",
		"PBR/Rock/Rock_Metallic.png",
		"PBR/Sand/Sand_Roughness.png", // Rock didn't come with roughness map
		//"PBR/Rock/Rock_AO.png"
		"PBR/Rock/Rock_Albedo.png"
	};

	CreateTexturedMaterial(*m_RockPBRPipeline, rockTextures, "Rock (PBR)");
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

	ReloadMeshes();

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

	// Update camera extent every frame if it's changed
	m_Camera->SetScreenDimensions(static_cast<float>(extent.width), static_cast<float>(extent.height));

	VkMesh* lastMesh = nullptr;
	VkMaterial* lastMaterial = nullptr;
	for (size_t i = 0; i < m_RenderableModels.size(); ++i)
	{
		VkModel*& model = m_RenderableModels[i];
		VkMaterial* modelMaterial = model->m_Material;
		WAVE_ASSERT(modelMaterial, "Attempting to render a model without a material");

		const VkShaderPipeline* modelPipeline = modelMaterial->GetShaderPipelinePtr();
		std::vector<vkn::VmaAllocatedDescriptorSet>& modelUniformBuffers = model->m_UniformBuffers;

		glm::mat4& mm = model->m_ModelMatrix;

		// Bind material if it's different than the last
		if (modelMaterial != lastMaterial)
		{
			modelPipeline->Bind(commandBuffer);
			lastMaterial = modelMaterial;
		}

		// Bind the mesh if it's different than the last
		if (&model->m_Mesh != lastMesh)
		{
			model->Bind(commandBuffer);
			lastMesh = &model->m_Mesh;
		}

		// Bind textures if they exist on the material
		if (modelMaterial->m_IsTexturedMaterial)
		{
			modelMaterial->BindMaterialTextures(commandBuffer);
		}

		// Submit push constants (constant uniforms)
		VkMeshPushConstants constants;
		constants.m_CameraPosition = glm::vec4(m_Camera->GetPosition(), 0.0f);
		constants.m_ModelMatrix = mm;
		vkCmdPushConstants(commandBuffer, modelPipeline->m_PipelineLayout, (VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT), 0, sizeof(VkMeshPushConstants), &constants);

		// Submit uniforms through descriptor sets
		// Currently padding/striping properties together in shaders to reduce complexity of descriptor sets
		VkMeshUniformBufferObject uniform;

		// Retrieve values from surf store
		glm::vec3 albedo = wv::SurfStore::s_SurfAlbedo;
		float metallic = wv::SurfStore::s_SurfMetallic;
		float roughness = wv::SurfStore::s_SurfRoughness;
		float ao = wv::SurfStore::s_SurfAO;
		glm::vec3 lightPosition = wv::SurfStore::s_SurfLightPosition;
		glm::vec3 lightColor = wv::SurfStore::s_SurfLightColor;

		// PBR uniforms  
		uniform.m_LightPosition = glm::vec4(lightPosition, 0.0f);
		uniform.m_LightColor = glm::vec4(lightColor, 0.0f);
		uniform.m_Albedo = glm::vec4(albedo, 0.0f);
		// x - metallic, y - roughness, z - ao, w - unused
		uniform.m_PBRSettings = glm::vec4(metallic, roughness, ao, 0.0f);

		// View uniforms
		uniform.m_ProjectionMatrix = m_Camera->GetProjectionMatrix();
		uniform.m_ViewMatrix = m_Camera->GetViewMatrix();

		void* data;
		VK_CALL(vmaMapMemory(m_VkHardware->m_VmaAllocator, modelUniformBuffers[m_CurrentFrameIndex].m_Allocation, &data));
		memcpy(data, &uniform, sizeof(VkMeshUniformBufferObject));
		vmaUnmapMemory(m_VkHardware->m_VmaAllocator, modelUniformBuffers[m_CurrentFrameIndex].m_Allocation);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, modelPipeline->m_PipelineLayout, 0, 1, &modelUniformBuffers[m_CurrentFrameIndex].m_Descriptor, 0, nullptr);

		// Draw geometry
		model->Draw(commandBuffer);
	}
}

void vkn::VkRenderer::EndRenderPass(VkCommandBuffer commandBuffer)
{
	WAVE_ASSERT(m_IsFrameStarted, "Cannot end render pass without a frame in progress");
	WAVE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Cannot render pass on a command buffer from a different frame");

	// Pass command buffer information to ImGui
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	vkCmdEndRenderPass(commandBuffer);
}

void vkn::VkRenderer::RegisterSurfSymbols()
{
	wv::SurfEngine::RegisterFunction("surf_SampleCFunction", (surf_fun_t) &surf_SampleCFunction);
	wv::SurfEngine::RegisterFunction("surf_SampleCFunctionWithArgs", (surf_fun_t) &surf_SampleCFunctionWithArgs);
}

void vkn::VkRenderer::InitImGui()
{
	VkDescriptorPoolSize imguiDescriptorPoolSizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo imguiPoolInfo = vkn::InitDescriptorPoolCreateInfo(static_cast<uint32_t>(std::size(imguiDescriptorPoolSizes)), 
		imguiDescriptorPoolSizes, 1000, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
	VK_CALL(vkCreateDescriptorPool(m_VkHardware->m_LogicalDevice, &imguiPoolInfo, nullptr, &m_ImguiDescriptorPool));

	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForVulkan(m_Window->GetSDLWindow());
	ImGui_ImplVulkan_InitInfo imguiInitInfo = ImGui_ImplVulkan_InitInfo();
	imguiInitInfo.Instance = m_VkHardware->m_Instance;
	imguiInitInfo.PhysicalDevice = m_VkHardware->m_PhysicalDevice;
	imguiInitInfo.Device = m_VkHardware->m_LogicalDevice;
	imguiInitInfo.Queue = m_VkHardware->m_GraphicsQueue;
	imguiInitInfo.DescriptorPool = m_ImguiDescriptorPool;
	imguiInitInfo.MinImageCount = 3;
	imguiInitInfo.ImageCount = 3;
	imguiInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	ImGui_ImplVulkan_Init(&imguiInitInfo, m_DefaultPass->m_RenderPass);

	SubmitToRenderer([&, this](VkCommandBuffer commandBuffer) {
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
	});
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void vkn::VkRenderer::LoadMeshes()
{
	core::Log(ELogType::Info, "[VkRenderer] Loading available meshes... this might take a while");

	m_SphereMesh.LoadFromObj(core::FileSystem::GetAssetPath("sphere.obj").string().c_str());
	m_VikingRoomMesh.LoadFromObj(core::FileSystem::GetAssetPath("viking_room.obj").string().c_str());
	m_RockMesh.LoadFromObj(core::FileSystem::GetAssetPath("rock.obj").string().c_str());

	m_TeapotMesh.LoadFromObj(core::FileSystem::GetAssetPath("teapot.obj").string().c_str());
	m_BunnyMesh.LoadFromObj(core::FileSystem::GetAssetPath("stanford_bunny.obj").string().c_str());
	m_SuzanneMesh.LoadFromObj(core::FileSystem::GetAssetPath("suzanne.obj").string().c_str());
	m_DragonMesh.LoadFromObj(core::FileSystem::GetAssetPath("xyzrgb_dragon.obj").string().c_str());

	m_Meshes.emplace("Sphere", m_SphereMesh);
	m_Meshes.emplace("Viking Room", m_VikingRoomMesh);
	m_Meshes.emplace("Rock", m_RockMesh);

	m_Meshes.emplace("Teapot", m_TeapotMesh);
	m_Meshes.emplace("Bunny", m_BunnyMesh);
	m_Meshes.emplace("Suzanne", m_SuzanneMesh);
	m_Meshes.emplace("Dragon", m_DragonMesh);

	ReloadMeshes();
}

void vkn::VkRenderer::ReloadMeshes()
{
	using String = core::StringHelpers;
	const std::string& selectedModel = wv::ToolPanel::s_SelectedModel;
	const std::string& selectedMaterial = wv::ToolPanel::s_SelectedMaterial;

	// Bail out if model or material is unchanged
	if (String::Equals(m_SelectedModel, selectedModel) && String::Equals(m_SelectedMaterial, selectedMaterial))
		return;

	// Wait for device to release command buffers
	VK_CALL(vkDeviceWaitIdle(m_VkHardware->m_LogicalDevice));

	// Reassign selected model
	if (m_SelectedModel != selectedModel)
		m_SelectedModel = selectedModel;

	if (m_SelectedMaterial != selectedMaterial)
		m_SelectedMaterial = selectedMaterial;

	// Lookup mesh and material and verify they exist
	VkMesh* targetMesh = LookupMesh(m_SelectedModel);
	if (!targetMesh)
	{
		core::Log(ELogType::Warn, "Failed to update mesh because \"{}\" doesn't seem exist", m_SelectedModel);
		return;
	}

	VkMaterial* targetMaterial = LookupMaterial(m_SelectedMaterial);
	if (!targetMaterial)
	{
		core::Log(ELogType::Warn, "Failed to update material because \"{}\" doesn't seem to exist", m_SelectedMaterial);
		return;
	}

	// Destroy pre-existing model data
	delete m_LoadedModel;
	m_RenderableModels.clear();

	// Allocate new model with target mesh/material
	m_LoadedModel = new VkModel(*this, *targetMesh, targetMaterial);

	// More models could be pushed back here for rendering if needed
	m_RenderableModels.push_back(m_LoadedModel);
}

vkn::VkMesh* vkn::VkRenderer::LookupMesh(const std::string& meshName)
{
	auto it = m_Meshes.find(meshName);

	if (it == m_Meshes.end())
		return nullptr;

	return &(*it).second;
}

vkn::VkMaterial* vkn::VkRenderer::LookupMaterial(const std::string& materialName)
{
	auto it = m_Materials.find(materialName);

	if (it == m_Materials.end())
		return nullptr;

	return &(*it).second;
}

vkn::VkMaterial* vkn::VkRenderer::CreateMaterial(const VkShaderPipeline& shaderPipeline, 
	const std::string& materialName)
{
	m_Materials.emplace(std::piecewise_construct,
		std::forward_as_tuple(materialName),
		std::forward_as_tuple(*this, shaderPipeline));

	return &m_Materials[materialName];
}

vkn::VkMaterial* vkn::VkRenderer::CreateTexturedMaterial(const VkShaderPipeline& shaderPipeline, 
	const std::vector<std::string>& textureNames, 
	const std::string& materialName)
{
	m_Materials.emplace(std::piecewise_construct,
		std::forward_as_tuple(materialName),
		std::forward_as_tuple(*this, shaderPipeline, textureNames));

	return &m_Materials[materialName];
}
