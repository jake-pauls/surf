#pragma once

#include <vector>

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
	/// @brief Implementation for Vulkan renderer
	class VkRenderer final : public wv::Renderer
	{
		friend class VkPass;
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

	private:
		void CreateCommandBuffers();

		VkCommandBuffer BeginFrame();
		void EndFrame();

		void BeginRenderPass(VkCommandBuffer commandBuffer);
		void DrawCommandBuffer(VkCommandBuffer commandBuffer);
		void EndRenderPass(VkCommandBuffer commandBuffer);

		// TODO: Abstract mesh loading
		void LoadMeshes();
		void UploadMesh(VkMesh& mesh);

	private:
		wv::Window* m_Window = nullptr;

		// Default Hardware
		VkHardware* m_VkHardware = nullptr;
		VkSwapChain* m_VkSwapChain = nullptr;
		VmaAllocator m_VmaAllocator = nullptr;
		std::vector<VkCommandBuffer> m_CommandBuffers = {};

		// Passes/Pipelines
		VkPass* m_DefaultPass = nullptr;
		VkShaderPipeline* m_DefaultPipeline = nullptr;

		// Frame Management
		bool m_IsFrameStarted = false;
		int m_CurrentFrameIndex = 0;
		uint32_t m_CurrentImageIndex = 0;

		bool m_FramebufferResized = false;

		// Temp
		VkMesh m_TriangleMesh = {};
		VkMesh m_SuzanneMesh = {};
	};
}
