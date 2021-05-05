#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class App
{
public:
	/// Initialize Vulkan Instance, query physical devices and their properties, features
	/// return initialization result
	VkResult init();

	/// Destroy Vulkan Instance
	void deinit();
private:
	VkResult queryPhysicalDevices();
	VkResult queryPhysicalDeviceProperties();
	VkResult queryPhysicalDeviceFeatures();

	VkInstance mInstance;
	std::vector<VkPhysicalDevice> mPhysicalDevices;
	std::vector<VkPhysicalDeviceProperties> mPhysicalDeviceProperties;
	std::vector<VkPhysicalDeviceFeatures> mPhysicalDeviceFeatures;
};

