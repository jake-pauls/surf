#pragma once

#include <vulkan/vulkan.hpp>

#include "IRenderer.h"
#include "VkHardware.h"
#include "VkSwapChain.h"

namespace wv
{
	class Window;
}

namespace vkn
{
	/// @brief Implementation for Vulkan renderer
	class VkRenderer final : public wv::IRenderer
	{
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
	};
}
