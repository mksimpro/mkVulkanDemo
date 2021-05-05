#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class App
{
public:
	/// Initialize Vulkan Instance,
	/// query physical devices and their properties, features, memory properties
	/// return initialization result
	VkResult init();

	/// Destroy Vulkan Instance
	void deinit();
private:
	VkResult queryPhysicalDevices();
	VkResult queryPhysicalDeviceProperties();
	VkResult queryPhysicalDeviceFeatures();
	VkResult queryPhysicalDeviceMemoryProperties();

	VkInstance mInstance;
	std::vector<VkPhysicalDevice> mPhysicalDevices;
	std::vector<VkPhysicalDeviceProperties> mPhysicalDeviceProperties;
	std::vector<VkPhysicalDeviceFeatures> mPhysicalDeviceFeatures;
	std::vector<VkPhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;
};

