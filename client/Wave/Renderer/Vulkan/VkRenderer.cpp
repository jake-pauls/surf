#include "VkRenderer.h"

#include <SDL3/SDL_vulkan.h>

#include <vector>
#include <map>

#include "VkRendererContext.h"

vkn::VkRenderer::VkRenderer(wv::Window* window)
	: m_Window(window)
	, m_VkInstance(nullptr)
{
}

void vkn::VkRenderer::Init()
{
	core::Log(ELogType::Info, "[VkRenderer] Initialized Vulkan renderer specification");

	// Initialize validation layers
	bool initializedValidationLayers = InitValidationLayers();
	WAVE_ASSERT(initializedValidationLayers, "Unsuccessfully retrieved required validation layers");

	// Create VkInstance
	CreateInstance();
	
	// Select a physical device
	SelectPhysicalDevice();
}

void vkn::VkRenderer::Teardown() const
{
	vkDestroyInstance(m_VkInstance, nullptr);
}

void vkn::VkRenderer::Clear() const
{
}

void vkn::VkRenderer::ClearColor() const
{
}

void vkn::VkRenderer::CreateInstance(bool listAvailableExtensions /* = false */)
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

bool vkn::VkRenderer::InitValidationLayers() const
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

		for (const VkLayerProperties& layerProperties : availableLayers)
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

void vkn::VkRenderer::SelectPhysicalDevice()
{
	uint32_t deviceCount = 0;
	VK_CALL(vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, nullptr));
	std::vector<VkPhysicalDevice> vkPhysicalDevices = std::vector<VkPhysicalDevice>(deviceCount);
	VK_CALL(vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, vkPhysicalDevices.data()));

	// Map device scores to their handles
	std::multimap<int, VkPhysicalDevice> candidateDevices = std::multimap<int, VkPhysicalDevice>();
	for (const VkPhysicalDevice& device : vkPhysicalDevices)
	{
		int deviceScore = GetDeviceScore(device);
		candidateDevices.emplace(deviceScore, device);
	}

	// First result in a sorted multimap should always be the most 'suitable' device
	if (candidateDevices.begin()->first > 0)
	{
		m_VkPhysicalDevice = candidateDevices.begin()->second;

		// TODO: Abstract device into sepearte class and retrieve props/features inherently
		VkPhysicalDeviceProperties vkPhysicalDeviceProps = VkPhysicalDeviceProperties();
		vkGetPhysicalDeviceProperties(m_VkPhysicalDevice, &vkPhysicalDeviceProps);
		VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = VkPhysicalDeviceFeatures();
		vkGetPhysicalDeviceFeatures(m_VkPhysicalDevice, &vkPhysicalDeviceFeatures);

		core::Log(ELogType::Info, "[VkRenderer] Selected '{}' as the physical device", vkPhysicalDeviceProps.deviceName);
	}

	WAVE_ASSERT(m_VkPhysicalDevice, "Failed to locate a suitable physical device");
}

unsigned int vkn::VkRenderer::GetDeviceScore(VkPhysicalDevice device) const 
{ 
	VkPhysicalDeviceProperties vkPhysicalDeviceProps = VkPhysicalDeviceProperties();
	vkGetPhysicalDeviceProperties(device, &vkPhysicalDeviceProps);
	VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = VkPhysicalDeviceFeatures();
	vkGetPhysicalDeviceFeatures(device, &vkPhysicalDeviceFeatures);

	// Cannot function without geometry shaders
	WAVE_ASSERT(vkPhysicalDeviceFeatures.geometryShader, "Physical device cannot process geometry");
	if (!vkPhysicalDeviceFeatures.geometryShader)
		return 0;

	// Cannot function without a valid graphics queue family
	QueueFamily deviceQueueFamily = FindQueueFamilies(device);
	WAVE_ASSERT(deviceQueueFamily.HasGraphicsFamily(), "Physical device doesn't have a valid graphics queue family");
	if (!deviceQueueFamily.HasGraphicsFamily())
		return 0;

	// Score can be measured by a variety of engine requirements
	// At the moment the only measure of score is the GPU type
	unsigned int score = 0;

	switch (vkPhysicalDeviceProps.deviceType)
	{
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:	  // (Most) Hardware GPUs
		score += 3;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:  // GPUs on a chip (ie: Laptops)
		score += 2;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:	  // GPUs virtualized (ie: through VirtualBox or a VM)
		score += 1;
		break;
	default:
		WAVE_ASSERT(false, "Unknown GPU type, the GPU type should be added as an applicable device before use")
		break;
	}

	return score;
}

vkn::QueueFamily vkn::VkRenderer::FindQueueFamilies(VkPhysicalDevice device) const
{
	QueueFamily result;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> vkQueueFamilies = std::vector<VkQueueFamilyProperties>(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, vkQueueFamilies.data());

	unsigned int i = 0;
	for (const VkQueueFamilyProperties& queueFamily : vkQueueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			result.m_GraphicsFamily = i;

		if (result.HasGraphicsFamily())
			break;

		i++;
	}

	return result;
}
