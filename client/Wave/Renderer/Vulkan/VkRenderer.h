#pragma once

#include <vector>
#include <functional>
#include <unordered_map>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "Renderer.h"
#include "VkMesh.h"
#include "VkPass.h"
#include "VkHardware.h"
#include "VkMaterial.h"
#include "VkSwapChain.h"
#include "VkShaderPipeline.h"

namespace wv
{
	class Window;
	class Camera;
}

namespace vkn
{
	class VkModel;

	/// @brief Context for uploading a render command to the renderer
	struct RendererUploadContext
	{
		VkFence m_UploadFence;
		VkCommandPool m_CommandPool;
		VkCommandBuffer m_CommandBuffer;
	};

	struct Renderable
	{
		VkMesh* m_Mesh;
		VkMaterial* m_Material;
	};

	/// @brief Implementation for Vulkan renderer
	class VkRenderer final : public wv::Renderer
	{
		friend class VkPass;
		friend class VkModel;
		friend class VkMaterial;
		friend class VkHardware;
		friend class VkSwapChain;
		friend class VkShaderPipeline;

	public:
		explicit VkRenderer(wv::Window* window, wv::Camera* camera);
		~VkRenderer() override = default;

		void Init() override;

		void Draw() override;

		void Teardown() override;

		inline void Clear() const override { WAVE_ASSERT(false, "Clear() is unimplemented for the VulkanRenderer"); }

	public:
		inline VkCommandBuffer GetCurrentCommandBuffer() const { return m_CommandBuffers[m_CurrentFrameIndex]; };

		/// @brief Intermediate function that submits commands to the renderer, performing all required hardware/command preprocesing 
		///		   before executing the passed command or set of commands
		/// @param submitFunction Function using a command buffer to execute a rendering command
		void SubmitToRenderer(std::function<void(VkCommandBuffer)>&& submitFunction) const;

		void ReloadMeshes();

	private:
		/// @brief Creates required synchronization objects for the renderer
		void CreateSyncObjects();

		/// @brief Creates a set of command buffers and pools 
		/// @note Runtime commands use c_MaxFramesInFlight to manage frames 
		void CreateCommands();

		VkCommandBuffer BeginFrame();
		void EndFrame();

		void BeginRenderPass(VkCommandBuffer commandBuffer);
		void DrawCommandBuffer(VkCommandBuffer commandBuffer);
		void EndRenderPass(VkCommandBuffer commandBuffer);

		void InitImGui();

		void LoadMeshes();

		VkMesh* LookupMesh(const std::string& meshName);
		VkMaterial* LookupMaterial(const std::string& materialName);
		VkMaterial* CreateMaterial(const VkShaderPipeline& shaderPipeline, 
			const std::string& materialName);
		VkMaterial* CreateTexturedMaterial(const VkShaderPipeline& shaderPipeline, 
			const std::string& textureName, 
			const std::string& materialName);

	public:
		uint32_t c_MaxFramesInFlight = 2;

	private:
		wv::Window* m_Window = nullptr;
		wv::Camera* m_Camera = nullptr;

		RendererUploadContext m_UploadContext = {};

		// Default Hardware
		VkHardware* m_VkHardware = nullptr;
		VkSwapChain* m_VkSwapChain = nullptr;

		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> m_CommandBuffers = {};

		// Passes/Pipelines
		VkPass* m_DefaultPass = nullptr;
		VkShaderPipeline* m_TexturedPipeline = nullptr;
		VkShaderPipeline* m_UntexturedPipeline = nullptr;

		// Frame Management
		bool m_IsFrameStarted = false;
		int m_CurrentFrameIndex = 0;
		uint32_t m_CurrentImageIndex = 0;

		// Synchronization
		std::vector<VkSemaphore> m_ImageAvailableSemaphores = {};
		std::vector<VkSemaphore> m_RenderFinishedSemaphores = {};
		std::vector<VkFence> m_InFlightFences = {};

		// Temp
		std::string m_SelectedModel = "";
		VkModel* m_UntexturedModel = nullptr;
		VkModel* m_LoadedModel = nullptr;

		VkMesh m_VikingRoomMesh = {};
		VkMesh m_TeapotMesh = {};
		VkMesh m_BunnyMesh = {};
		VkMesh m_SuzanneMesh = {};
		VkMesh m_DragonMesh = {};


		VkDescriptorPool m_ImguiDescriptorPool;

		std::vector<VkModel*> m_RenderableModels;
		std::unordered_map<std::string, VkMesh> m_Meshes;
		std::unordered_map<std::string, VkMaterial> m_Materials;
	};
}
