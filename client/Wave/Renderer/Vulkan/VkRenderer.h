#pragma once

#include <vector>
#include <functional>

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

#include "Renderer.h"
#include "VkMesh.h"
#include "VkPass.h"
#include "VkHardware.h"
#include "VkSwapChain.h"
#include "VkShaderPipeline.h"

namespace wv
{
	class Window;
}

namespace vkn
{
	class VkModel;

	struct RendererUploadContext
	{
		VkFence m_UploadFence;
		VkCommandPool m_CommandPool;
		VkCommandBuffer m_CommandBuffer;
	};

	/// @brief Implementation for Vulkan renderer
	class VkRenderer final : public wv::Renderer
	{
		friend class VkPass;
		friend class VkModel;
		friend class VkHardware;
		friend class VkSwapChain;
		friend class VkShaderPipeline;

	public:
		explicit VkRenderer(wv::Window* window);
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

	private:
		/// @brief Creates required synchronization objects for the renderer
		void CreateSyncObjects();

		/// @brief Creates a set of command buffers and pools 
		/// @note Runtime commands use c_MaxFramesInFlight to manage frames 
		void CreateCommands();

		/// @brief Creates uniform buffers and allocates their associating descriptor sets
		void CreateUniformBuffers();

		VkCommandBuffer BeginFrame();
		void EndFrame();

		void BeginRenderPass(VkCommandBuffer commandBuffer);
		void DrawCommandBuffer(VkCommandBuffer commandBuffer);
		void EndRenderPass(VkCommandBuffer commandBuffer);

		// TODO: Abstract mesh loading
		void LoadMeshes();
		void CreateTexture(const std::string& filename);

	private:
		wv::Window* m_Window = nullptr;

		RendererUploadContext m_UploadContext = {};

		// Default Hardware
		VkHardware* m_VkHardware = nullptr;
		VkSwapChain* m_VkSwapChain = nullptr;

		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> m_CommandBuffers = {};

		// Passes/Pipelines
		VkPass* m_DefaultPass = nullptr;
		VkShaderPipeline* m_DefaultPipeline = nullptr;

		// Frame Management
		bool m_IsFrameStarted = false;
		int m_CurrentFrameIndex = 0;
		uint32_t m_CurrentImageIndex = 0;

		// Synchronization
		uint32_t c_MaxFramesInFlight = 2;
		std::vector<VkSemaphore> m_ImageAvailableSemaphores = {};
		std::vector<VkSemaphore> m_RenderFinishedSemaphores = {};
		std::vector<VkFence> m_InFlightFences = {};

		// Temp
		std::vector<VmaAllocatedDescriptorSet> m_UniformBuffers = {};
		VkModel* m_LoadedModel = nullptr;
		VkMesh m_LoadedMesh = {};

		VkTexture m_Texture = {};
	};
}
