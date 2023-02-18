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

		void Clear() const override;

		void ClearColor() const override;

	private:
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		// TODO: Abstract mesh loading
		void LoadMeshes();
		void UploadMesh(VkMesh& mesh);

	private:
		wv::Window* m_Window = nullptr;

		// Default Hardware
		VkHardware* m_VkHardware = nullptr;
		VkSwapChain* m_VkSwapChain = nullptr;
		VmaAllocator m_VmaAllocator = nullptr;

		// Passes/Pipelines
		VkPass* m_DefaultPass = nullptr;
		VkShaderPipeline* m_TrianglePipeline = nullptr;

		// Temp
		VkMesh m_TriangleMesh;
		VkMesh m_SuzanneMesh;

		// Frame Management
		const int c_MaxFramesInFlight = 2;
		uint32_t m_CurrentFrame = 0;

		// Detect Framebuffer Resizing
		bool m_FramebufferResized = false;

		// Synchronization
		std::vector<VkSemaphore> m_ImageAvailableSemaphores = {};
		std::vector<VkSemaphore> m_RenderFinishedSemaphores = {};
		std::vector<VkFence> m_InFlightFences = {};
	};
}
