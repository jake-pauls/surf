#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "Renderer.h"
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

	private:
		wv::Window* m_Window = nullptr;

		VkHardware* m_VkHardware = nullptr;
		VkSwapChain* m_VkSwapChain = nullptr;
		VkPass* m_DefaultPass = nullptr;
		VkShaderPipeline* m_TrianglePipeline = nullptr;

		// Refactor
		const int c_MaxFramesInFlight = 2;
		uint32_t m_CurrentFrame = 0;
		bool m_FramebufferResized = false;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores = {};
		std::vector<VkSemaphore> m_RenderFinishedSemaphores = {};
		std::vector<VkFence> m_InFlightFences = {};
	};
}
