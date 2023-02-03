#pragma once

#include <vulkan/vulkan.hpp>

#include "Window.h"
#include "IRenderer.h"
#include "VkHardware.h"

namespace vkn
{
	/// @brief Implementation for Vulkan renderer
	class VkRenderer final : public wv::IRenderer
	{
	public:
		explicit VkRenderer(wv::Window* window);
		~VkRenderer() override;

		void Init() override;

		void Teardown() const override;

		void Clear() const override;

		void ClearColor() const override;

	private:
		wv::Window* m_Window = nullptr;
		VkHardware* m_VkHardware = nullptr;
	};
}
