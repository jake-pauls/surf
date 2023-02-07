#pragma once

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
		friend class VkShaderPipeline;

	public:
		explicit VkRenderer(wv::Window* window);
		~VkRenderer() override = default;

		void Init() override;

		void Teardown() override;

		void Clear() const override;

		void ClearColor() const override;

	private:
		wv::Window* m_Window = nullptr;

		VkHardware m_VkHardware;
		VkSwapChain m_VkSwapChain;

		// Temp
		VkPass* m_PassthroughPass = nullptr;
		VkShaderPipeline* m_PassthroughPipeline = nullptr;
	};
}
