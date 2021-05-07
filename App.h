#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class App
{
public:
	/// Initialize Vulkan Instance. Query physical devices and their
	/// properties, features, memory properties, queue familty properties.
	/// Query instance layers and device layers.
	/// return initialization result
	VkResult init();

	/// Destroy Vulkan Instance, logical device
	VkResult deinit();
private:
	VkResult queryInstanceLayerProperties();
	VkResult queryPhysicalDevices();
	VkResult queryPhysicalDeviceLayerProperties();
	VkResult queryPhysicalDeviceProperties();
	VkResult queryPhysicalDeviceFeatures();
	VkResult queryPhysicalDeviceMemoryProperties();
	void queryPhysicalDeviceQueueFamilyProperties();

	bool getQueueFamilyIndex(size_t physicalDeviceIndex, VkQueueFlags queueFlags, uint32_t& queueFamilyIndex);
	VkResult createLogicalDevice(size_t physicalDeviceIndex, uint32_t queueFamilyIndex);

	VkInstance mInstance;
	std::vector<VkLayerProperties> mInstanceLayerProperties;
	std::vector<VkPhysicalDevice> mPhysicalDevices;
	std::vector<std::vector<VkLayerProperties>> mPhysicalDeviceLayerProperties;
	std::vector<VkPhysicalDeviceProperties> mPhysicalDeviceProperties;
	std::vector<VkPhysicalDeviceFeatures> mPhysicalDeviceFeatures;
	std::vector<VkPhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;
	std::vector<std::vector<VkQueueFamilyProperties>> mQueueFamilyProperties;

	VkDevice mDevice;
};

