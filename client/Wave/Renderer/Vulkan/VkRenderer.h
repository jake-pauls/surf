#pragma once

#include <vulkan/vulkan.hpp>

#include "IRenderer.h"
#include "Window.h"

namespace wvk
{
	/// @brief Implementation for Vulkan renderer
	class VkRenderer final : public wv::IRenderer
	{
	public:
		explicit VkRenderer(wv::Window* window);
		~VkRenderer() override = default;

		void Init() override;

		void Teardown() const override;

		void Clear() const override;

		void ClearColor() const override;

	private:
		/// @brief Creates the Vulkan instance and loads required extensions from SDL
		/// @param listAvailableExtensions Optionally list available Vulkan extensions on instance creation, default is false
		void CreateInstance(bool listAvailableExtensions = false);

		/// @brief Checks validation layer support and initializes them if possible 
		bool InitValidationLayers() const;

	private:
		wv::Window* m_Window = nullptr;

		VkInstance m_VkInstance;

#ifdef _WAVE_DEBUG
		const bool m_ValidationLayersEnabled = true;
		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
#else
		const bool m_ValidationLayersEnabled = false;
		const std::vector<const char*> m_ValidationLayers = {};
#endif
	};
}
