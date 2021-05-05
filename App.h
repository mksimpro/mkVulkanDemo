#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class App
{
public:
	/// Initialize Vulkan Instance. Query physical devices and their
	/// properties, features, memory properties, queue familty properties.
	/// return initialization result
	VkResult init();

	/// Destroy Vulkan Instance
	void deinit();
private:
	VkResult queryPhysicalDevices();
	VkResult queryPhysicalDeviceProperties();
	VkResult queryPhysicalDeviceFeatures();
	VkResult queryPhysicalDeviceMemoryProperties();
	void queryPhysicalDeviceQueueFamilyProperties();

	VkInstance mInstance;
	std::vector<VkPhysicalDevice> mPhysicalDevices;
	std::vector<VkPhysicalDeviceProperties> mPhysicalDeviceProperties;
	std::vector<VkPhysicalDeviceFeatures> mPhysicalDeviceFeatures;
	std::vector<VkPhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;
	std::vector<std::vector<VkQueueFamilyProperties>> mQueueFamilyProperties;
};

