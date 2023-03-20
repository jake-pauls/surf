#pragma once

#include <vector>
#include <optional>

#include <vk_mem_alloc.h>

namespace wv
{
	class Window;
}

namespace vkn
{
	class VkModel;
	class VkMaterial;
	class VkRenderer;
	class VkSwapChain;
	class VkShaderPipeline;

	struct VmaAllocatedBuffer;

	/// @brief Indices for various queue families available in Vulkan devices
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
		friend class VkModel;
		friend class VkMaterial;
		friend class VkRenderer;
		friend class VkSwapChain;
		friend class VkShaderPipeline;

	public:
		explicit VkHardware(wv::Window* window);
		~VkHardware();

		void Teardown();

	private:
		/// @brief Debug messenger callback for triggered validation layers
		static VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			core::Log(ELogType::Error, "[VkDebugMessenger] {}", pCallbackData->pMessage);
			return VK_FALSE;
		}

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

		/// @brief Creates the VMA allocator for buffer allocations
		void CreateVMAAllocator();

		/// @brief Logic to find graphics queue families 
		/// @param device The VkPhysicalDevice to retrieve the queue families of
		/// @return The corresponding queue family of the physical device
		QueueFamily FindQueueFamilies(const VkPhysicalDevice& device) const;

		/// @brief Logic to check if the physical device has required extensions
		/// @param device The VkPhysicalDevice to retrieve the extensions of
		/// @return True if the passed device has the required extensions
		bool HasRequiredExtensions(const VkPhysicalDevice& device) const;

		/// @brief Evaluates the amount at which a particular VkPhysicalDevice is suitable for use 
		/// @param device The VkPhysicalDevice to check
		/// @return Retrieves a score for the device based on its available features and context
		unsigned int GetDeviceScore(const VkPhysicalDevice& device) const;

		/// @brief Allocates a buffer type using VMA
		/// @param buffer Reference to a VmaAllocatedBuffer to allocate into
		/// @param size Size of the buffer to allocate
		/// @param usageFlags Corresponding Vulkan usage flags for the buffer
		/// @param memoryUsage Corresponding VMA usage flags for the buffer
		void CreateVMABuffer(VmaAllocatedBuffer& buffer,
			size_t size,
			VkBufferUsageFlags usageFlags,
			VmaMemoryUsage memoryUsage) const;

	private:
		wv::Window* m_Window = nullptr;

		VkInstance m_Instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice = VK_NULL_HANDLE;

		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_PresentationQueue = VK_NULL_HANDLE;

		VmaAllocator m_VmaAllocator = nullptr;

		const std::vector<const char*> m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#ifdef _WAVE_DEBUG
		const bool c_ValidationLayersEnabled = true;
		const std::vector<const char*> m_ValidationLayers = { 
			"VK_LAYER_KHRONOS_validation",	// Core Validation 
			"VK_LAYER_LUNARG_monitor"		// Framerate Display on Win32
		};
#else
		const bool c_ValidationLayersEnabled = false;
		const std::vector<const char*> m_ValidationLayers = {};
#endif
	};
}
