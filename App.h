#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class App
{
public:
	/// Initialize Vulkan Instance, query physical devices
	/// return initialization result
	VkResult init();

	/// Destroy Vulkan Instance
	void deinit();
private:
	VkResult queryPhysicalDevices();

	VkInstance mInstance;
	std::vector<VkPhysicalDevice> mPhysicalDevices;
};

