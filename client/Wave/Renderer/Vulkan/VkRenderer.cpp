#include "VkRenderer.h"

#include <SDL3/SDL_vulkan.h>

#include <vector>

#include "VkRendererContext.h"

wvk::VkRenderer::VkRenderer(wv::Window* window)
	: m_Window(window)
	, m_VkInstance(nullptr)
{
}

void wvk::VkRenderer::Init()
{
	core::Log(ELogType::Info, "[VkRenderer] Initialized Vulkan renderer specification");

	// Initialize validation layers
	bool initializedValidationLayers = InitValidationLayers();
	WAVE_ASSERT(initializedValidationLayers, "Unsuccessfully retrieved required validation layers");

	// Create VkInstance
	CreateInstance();
}

void wvk::VkRenderer::Teardown() const
{
	vkDestroyInstance(m_VkInstance, nullptr);
}

void wvk::VkRenderer::Clear() const
{
}

void wvk::VkRenderer::ClearColor() const
{
}

void wvk::VkRenderer::CreateInstance(bool listAvailableExtensions /* = false */)
{
	SDL_Window* window = m_Window->GetSDLWindow();

	// Retrieve required SDL extensions from the renderer context
	VkRendererContext* vkContext = dynamic_cast<VkRendererContext*>(m_Window->GetRendererContext());
	std::vector<const char*> contextExtensions = vkContext->GetVulkanContextExtensions();

	// Initialize the application info
	VkApplicationInfo vkAppInfo = VkApplicationInfo();
	vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkAppInfo.pApplicationName = "wave.";
	vkAppInfo.applicationVersion = VK_MAKE_API_VERSION(1, 3, 236, 0);
	vkAppInfo.pEngineName = "No Engine";
	vkAppInfo.engineVersion = VK_MAKE_API_VERSION(1, 3, 236, 0);
	vkAppInfo.apiVersion = VK_API_VERSION_1_3;

	// Initialize the instance info
	VkInstanceCreateInfo vkCreateInfo = VkInstanceCreateInfo();
	vkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkCreateInfo.pApplicationInfo = &vkAppInfo;
	vkCreateInfo.enabledExtensionCount = static_cast<uint32_t>(contextExtensions.size());
	vkCreateInfo.ppEnabledExtensionNames = contextExtensions.data();

	core::Log(ELogType::Info, "[VkRenderer] Successfully loaded {} extension(s)", contextExtensions.size());

	// Ensure that the instance contains validation layers if they exist
	if (m_ValidationLayersEnabled)
	{
		vkCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
		vkCreateInfo.ppEnabledLayerNames = m_ValidationLayers.data();
	}
	else
	{
		vkCreateInfo.enabledLayerCount = 0;
	}

	if (listAvailableExtensions)
	{
		uint32_t totalExtensionCount = 0;
		VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &totalExtensionCount, nullptr));
		std::vector<VkExtensionProperties> totalExtensions = std::vector<VkExtensionProperties>(totalExtensionCount);
		VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &totalExtensionCount, totalExtensions.data()));

		core::Log(ELogType::Info, "[VkRenderer] Available extensions:");
		for (const auto& extension : totalExtensions)
			core::Log(ELogType::Info, "[VkRenderer] \t{}", extension.extensionName);
	}

	VK_CALL(vkCreateInstance(&vkCreateInfo, nullptr, &m_VkInstance));
	core::Log(ELogType::Info, "[VkRenderer] Created Vulkan instance");
}

bool wvk::VkRenderer::InitValidationLayers() const
{
	if (!m_ValidationLayersEnabled)
	{
		core::Log(ELogType::Warn, "[VkRenderer] Validation layers are disabled, skipping initialization");
		return true;
	}

	// Request available validation layers
	uint32_t vLayerCount = 0;
	VK_CALL(vkEnumerateInstanceLayerProperties(&vLayerCount, nullptr));
	std::vector<VkLayerProperties> availableLayers = std::vector<VkLayerProperties>(vLayerCount);
	VK_CALL(vkEnumerateInstanceLayerProperties(&vLayerCount, availableLayers.data()));

	// Ensure that all available layers match requested layers
	uint32_t vLayerLoadedCount = 0;
	for (const char* layerName : m_ValidationLayers)
	{
		bool isLayerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (core::StringHelpers::Equals(layerName, layerProperties.layerName))
			{
				isLayerFound = true;
				vLayerLoadedCount++;
				break;
			}
		}

		if (!isLayerFound)
		{
			core::Log(ELogType::Warn, "[VkRenderer] Failed to find requested validation layer: {}", layerName);
			return false;
		}
	}

	core::Log(ELogType::Info, "[VkRenderer] Successfully loaded {} validation layer(s)", vLayerLoadedCount);

	return true;
}
