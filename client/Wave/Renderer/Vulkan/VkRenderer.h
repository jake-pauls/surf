#pragma once

#include <vulkan/vulkan.hpp>

#include "Renderer.h"

namespace vk
{
	class VkRenderer final : public wv::Renderer
	{
	public:
		VkRenderer() = default;
		~VkRenderer() override = default;

		void Init() override;

		void Clear() const override;

		void ClearColor() const override;
	};
}
