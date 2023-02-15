#include "VkHardware.h"

#include <map>
#include <set>
#include <vector>

#include <SDL3/SDL_vulkan.h>

#include "VkInitializers.h"
#include "VkRendererContext.h"

vkn::VkHardware::VkHardware(wv::Window* window)
	: m_Window(window)
{
	core::Log(ELogType::Trace, "[VkHardware] Initializing Vulkan hardware");

	bool initializedValidationLayers = InitValidationLayers();
	WAVE_ASSERT(initializedValidationLayers, "Unsuccessfully retrieved required validation layers");

	CreateInstance();

	SelectPhysicalDevice();
	CreateLogicalDevice();

	CreateCommands();
}

vkn::VkHardware::~VkHardware()
{
	core::Log(ELogType::Trace, "[VkHardware] Tearing down Vulkan hardware");

	Teardown();
}

void vkn::VkHardware::Teardown()
{
	vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);
	vkDestroyDevice(m_LogicalDevice, nullptr);
	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

	auto pfnDestroyDebugMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
	if (pfnDestroyDebugMessenger)
		pfnDestroyDebugMessenger(m_Instance, m_DebugMessenger, nullptr);

	vkDestroyInstance(m_Instance, nullptr);
}

void vkn::VkHardware::CreateInstance(bool listAvailableExtensions /* = false */)
{
	SDL_Window* window = m_Window->GetSDLWindow();

	// Get the renderer context, if we're here it should be Vulkan
	VkRendererContext* vkContext = dynamic_cast<VkRendererContext*>(m_Window->GetRendererContext());

	// Retrieve required SDL extensions from the renderer context
	std::vector<const char*> contextExtensions = vkContext->GetVulkanContextExtensions(true);

	// Initialize the application info
	VkApplicationInfo vkAppInfo = VkApplicationInfo();
	vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkAppInfo.pApplicationName = "wave.";
	vkAppInfo.applicationVersion = VK_MAKE_API_VERSION(1, 3, 236, 0);
	vkAppInfo.pEngineName = "No Engine";
	vkAppInfo.engineVersion = VK_MAKE_API_VERSION(1, 3, 236, 0);
	vkAppInfo.apiVersion = VK_API_VERSION_1_3;

	// Initialize the instance info
	VkInstanceCreateInfo vkInstanceCreateInfo = VkInstanceCreateInfo();
	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pApplicationInfo = &vkAppInfo;
	vkInstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(contextExtensions.size());
	vkInstanceCreateInfo.ppEnabledExtensionNames = contextExtensions.data();

	core::Log(ELogType::Trace, "[VkHardware] Successfully loaded {} extension(s)", contextExtensions.size());
	
	VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = VkDebugUtilsMessengerCreateInfoEXT();

	// Ensure that the instance contains validation layers if they exist
	if (c_ValidationLayersEnabled)
	{
		// Initialize the debug messenger
		debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debugMessengerCreateInfo.pfnUserCallback = VkDebugMessengerCallback;
		debugMessengerCreateInfo.pUserData = nullptr;

		vkInstanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugMessengerCreateInfo;

		vkInstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
		vkInstanceCreateInfo.ppEnabledLayerNames = m_ValidationLayers.data();
	}
	else
	{
		vkInstanceCreateInfo.enabledLayerCount = 0;
	}

	if (listAvailableExtensions)
	{
		uint32_t totalExtensionCount = 0;
		VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &totalExtensionCount, nullptr));
		std::vector<VkExtensionProperties> totalExtensions = std::vector<VkExtensionProperties>(totalExtensionCount);
		VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &totalExtensionCount, totalExtensions.data()));

		core::Log(ELogType::Trace, "[VkHardware] Available extensions:");
		for (const VkExtensionProperties& extension : totalExtensions)
		{
			core::Log(ELogType::Trace, "[VkHardware] \t{}", extension.extensionName);
		}
	}

	VK_CALL(vkCreateInstance(&vkInstanceCreateInfo, nullptr, &m_Instance));
	core::Log(ELogType::Trace, "[VkHardware] Created Vulkan instance");

	// Setup the surface with SDL
	vkContext->SetupSDLVulkanSurface(m_Instance, &m_Surface);

	// Create debug messenger and attach it to newly created instance
	if (c_ValidationLayersEnabled)
	{
		auto pfnCreateDebugMessenger = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
		const auto result = pfnCreateDebugMessenger
			? pfnCreateDebugMessenger(m_Instance, &debugMessengerCreateInfo, nullptr, &m_DebugMessenger)
			: VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

bool vkn::VkHardware::InitValidationLayers() const
{
	if (!c_ValidationLayersEnabled)
	{
		core::Log(ELogType::Warn, "[VkHardware] Validation layers are disabled, skipping initialization");
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
			core::Log(ELogType::Warn, "[VkHardware] Failed to find requested validation layer: {}", layerName);
			return false;
		}
	}

	core::Log(ELogType::Trace, "[VkHardware] Successfully loaded {} validation layer(s)", vLayerLoadedCount);

	return true;
}

void vkn::VkHardware::SelectPhysicalDevice()
{
	uint32_t deviceCount = 0;
	VK_CALL(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr));
	std::vector<VkPhysicalDevice> vkPhysicalDevices = std::vector<VkPhysicalDevice>(deviceCount);
	VK_CALL(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, vkPhysicalDevices.data()));

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
		m_PhysicalDevice = candidateDevices.begin()->second;

		// TODO: Abstract device into sepearte class and retrieve props/features inherently
		VkPhysicalDeviceProperties vkPhysicalDeviceProps = VkPhysicalDeviceProperties();
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &vkPhysicalDeviceProps);
		VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = VkPhysicalDeviceFeatures();
		vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &vkPhysicalDeviceFeatures);

		core::Log(ELogType::Trace, "[VkHardware] Selected '{}' as the physical device", vkPhysicalDeviceProps.deviceName);
	}

	WAVE_ASSERT(m_PhysicalDevice, "Failed to locate a suitable physical device");
}

void vkn::VkHardware::CreateLogicalDevice()
{
	QueueFamily queueFamily = FindQueueFamilies(m_PhysicalDevice);

	// Setup valid queue families and initialize their information
	std::set<uint32_t> queueFamilies = { queueFamily.m_GraphicsFamily.value(), queueFamily.m_PresentationFamily.value()};
	std::vector<VkDeviceQueueCreateInfo> vkDeviceQueueCreateInfos = std::vector<VkDeviceQueueCreateInfo>();

	// Queue priorities influence the scheduling of command buffer execution (0.0 - 1.0)
	float queuePriority = 1.0f;

	for (uint32_t queueFamily : queueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = VkDeviceQueueCreateInfo();
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		vkDeviceQueueCreateInfos.push_back(queueCreateInfo);
	}

	// Enable/disable physical device features
	VkPhysicalDeviceFeatures vkDeviceFeatures = VkPhysicalDeviceFeatures();

	// Setup the logical device with the queue and physical device configuration
	VkDeviceCreateInfo vkDeviceCreateInfo = VkDeviceCreateInfo();
	vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vkDeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(vkDeviceQueueCreateInfos.size());
	vkDeviceCreateInfo.pQueueCreateInfos = vkDeviceQueueCreateInfos.data();
	vkDeviceCreateInfo.pEnabledFeatures = &vkDeviceFeatures;

	// Enable the swap chain and other device extensions
	vkDeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
	vkDeviceCreateInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();
	core::Log(ELogType::Trace, "[VkHardware] Enabled the swap chain");

	if (c_ValidationLayersEnabled)
	{
		vkDeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
		vkDeviceCreateInfo.ppEnabledLayerNames = m_ValidationLayers.data();
	}
	else
	{
		vkDeviceCreateInfo.enabledLayerCount = 0;
	}

	// Create the logical device
	VK_CALL(vkCreateDevice(m_PhysicalDevice, &vkDeviceCreateInfo, nullptr, &m_LogicalDevice));
	core::Log(ELogType::Trace, "[VkHardware] Created logical device");

	// Retrieve handles for each queue on the logical device
	vkGetDeviceQueue(m_LogicalDevice, queueFamily.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
	vkGetDeviceQueue(m_LogicalDevice, queueFamily.m_PresentationFamily.value(), 0, &m_PresentationQueue);
	if (m_GraphicsQueue && m_PresentationQueue)
		core::Log(ELogType::Trace, "[VkHardware] Created graphics and presentation queues");
}

void vkn::VkHardware::CreateCommands()
{
	QueueFamily queueFamily = FindQueueFamilies(m_PhysicalDevice);

	auto commandPoolInfo = vkn::InitCommandPoolCreateInfo(queueFamily.m_GraphicsFamily.value(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	VK_CALL(vkCreateCommandPool(m_LogicalDevice, &commandPoolInfo, nullptr, &m_CommandPool));

	// TODO: c_MaxFramesInFlight?
	m_CommandBuffers.resize(2);
	auto commandBufferAllocateInfo = vkn::InitCommandBufferAllocateInfo(m_CommandPool, 2);
	VK_CALL(vkAllocateCommandBuffers(m_LogicalDevice, &commandBufferAllocateInfo, m_CommandBuffers.data()));
}

vkn::QueueFamily vkn::VkHardware::FindQueueFamilies(const VkPhysicalDevice& device) const
{
	QueueFamily result;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> vkQueueFamilies = std::vector<VkQueueFamilyProperties>(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, vkQueueFamilies.data());

	unsigned int i = 0;
	for (const VkQueueFamilyProperties& queueFamily : vkQueueFamilies)
	{
		// Check for graphics queue family support
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			result.m_GraphicsFamily = i;

		// Check for presentation queue family support
		VkBool32 presentationSupport = false;
		VK_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentationSupport));
		if (queueFamily.queueCount > 0 && presentationSupport)
			result.m_PresentationFamily = i;

		if (result.HasGraphicsFamily() && result.HasPresentationFamily())
			break;

		i++;
	}

	return result;
}

bool vkn::VkHardware::HasRequiredExtensions(const VkPhysicalDevice& device) const
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions = std::vector<VkExtensionProperties>(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions = std::set<std::string>(m_DeviceExtensions.begin(), m_DeviceExtensions.end());
	for (const VkExtensionProperties& extension : availableExtensions)
	{
		// 'Check' extensions off the list as they become available
		requiredExtensions.erase(extension.extensionName);

		// Return early if all extensions have been found
		if (requiredExtensions.empty())
			break;
	}

	return requiredExtensions.empty();
}

unsigned int vkn::VkHardware::GetDeviceScore(const VkPhysicalDevice& device) const 
{ 
	VkPhysicalDeviceProperties vkPhysicalDeviceProps = VkPhysicalDeviceProperties();
	vkGetPhysicalDeviceProperties(device, &vkPhysicalDeviceProps);
	VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = VkPhysicalDeviceFeatures();
	vkGetPhysicalDeviceFeatures(device, &vkPhysicalDeviceFeatures);

	// Cannot function without geometry shaders
	WAVE_ASSERT(vkPhysicalDeviceFeatures.geometryShader, "Physical device cannot process geometry");
	if (!vkPhysicalDeviceFeatures.geometryShader)
		return 0;

	// Ensure there are valid graphics and presentation queue families
	QueueFamily deviceQueueFamily = FindQueueFamilies(device);

	WAVE_ASSERT(deviceQueueFamily.HasGraphicsFamily(), "Physical device doesn't have a valid graphics queue");
	if (!deviceQueueFamily.HasGraphicsFamily())
		return 0;

	WAVE_ASSERT(deviceQueueFamily.HasPresentationFamily(), "Physical device doesn't have a valid presentation queue");
	if (!deviceQueueFamily.HasPresentationFamily())
		return 0;

	// Ensure that the proper device extensions are supported
	bool extensionsSupported = HasRequiredExtensions(device);
	WAVE_ASSERT(extensionsSupported, "Physical device doesn't support the required extensions");
	if (!extensionsSupported)
		return 0;

	// Note: Swap chain support is currently being checked when a swap chain is created

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
		score = 0;
		break;
	}

	WAVE_ASSERT(score > 0, "Returning score for unsuitable GPU, undefined behaviour may occur if this GPU is selected");

	return score;
}
