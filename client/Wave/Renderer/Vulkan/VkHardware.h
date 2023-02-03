#pragma once

#include "Window.h"

namespace vkn
{
	/// @brief Struct containing indices for various queue families available in Vulkan devices
	struct QueueFamily 
	{
		/// @brief Graphics family supports drawing functions
		std::optional<uint32_t> m_GraphicsFamily;

		/// @brief Presentation family presents images to the VkSurface
		std::optional<uint32_t> m_PresentationFamily;

		inline bool HasGraphicsFamily() const { return m_GraphicsFamily.has_value(); }

		inline bool HasPresentationFamily() const { return m_PresentationFamily.has_value(); }
	};

	class VkHardware final
	{
	public:
		explicit VkHardware(wv::Window* window);
		~VkHardware() = default;

		void Init();

		void Teardown();

	private:
		/// @brief Creates the Vulkan instance and loads required extensions from SDL
		/// @param listAvailableExtensions Optionally list available Vulkan extensions on instance creation, default is false
		void CreateInstance(bool listAvailableExtensions = false);

		/// @brief Checks validation layer support and initializes them if possible 
		bool InitValidationLayers() const;

		/// @brief Selects one physical device appropriate for use
		void SelectPhysicalDevice();

		/// @brief Initializes the logical device interfacing with Vulkan
		void CreateLogicalDevice();

		/// @brief Logic to find graphics queue families 
		/// @param device The VkPhysicalDevice to retrieve the queue families of
		/// @return The corresponding queue family of the physical device
		QueueFamily FindQueueFamilies(VkPhysicalDevice device) const;

		/// @brief Evaluates the amount at which a particular VkPhysicalDevice is suitable for use 
		/// @param device The VkPhysicalDevice to check
		/// @return Retrieves a score for the device based on its available features and context
		unsigned int GetDeviceScore(VkPhysicalDevice device) const;

	private:
		wv::Window* m_Window = nullptr;
		VkHardware* m_VkHardware = nullptr;

		VkInstance m_Instance = VK_NULL_HANDLE;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice = VK_NULL_HANDLE;

		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_PresentationQueue = VK_NULL_HANDLE;

#ifdef _WAVE_DEBUG
		const bool c_ValidationLayersEnabled = true;
		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
#else
		const bool c_ValidationLayersEnabled = false;
		const std::vector<const char*> m_ValidationLayers = {};
#endif
	};
}
