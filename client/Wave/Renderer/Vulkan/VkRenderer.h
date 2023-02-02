#pragma once

#include <vulkan/vulkan.hpp>

#include "IRenderer.h"
#include "Window.h"

namespace vkn
{
	struct QueueFamily 
	{
		std::optional<uint32_t> m_GraphicsFamily;

		bool HasGraphicsFamily() const { return m_GraphicsFamily.has_value(); }
	};

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

		/// @brief Selects one physical device appropriate for use
		void SelectPhysicalDevice();

		/// @brief Evaluates the amount at which a particular VkPhysicalDevice is suitable for use 
		/// @param device The VkPhysicalDevice to check
		/// @return Retrieves a score for the device based on its available features and context
		unsigned int GetDeviceScore(VkPhysicalDevice device) const;

		/// @brief Logic to find graphics queue families 
		/// @param device The VkPhysicalDevice to retrieve the queue families of
		/// @return The corresponding queue family of the physical device
		QueueFamily FindQueueFamilies(VkPhysicalDevice device) const;

	private:
		wv::Window* m_Window = nullptr;

		VkInstance m_VkInstance = VK_NULL_HANDLE;
		VkPhysicalDevice m_VkPhysicalDevice = VK_NULL_HANDLE;

#ifdef _WAVE_DEBUG
		const bool m_ValidationLayersEnabled = true;
		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
#else
		const bool m_ValidationLayersEnabled = false;
		const std::vector<const char*> m_ValidationLayers = {};
#endif
	};
}
