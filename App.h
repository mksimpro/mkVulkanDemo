#pragma once

#include <vulkan/vulkan.h>

class App
{
public:
	/// Initialize Vulkan Instance
	/// return initialization result
	VkResult init();

	/// Destroy Vulkan Instance
	void deinit();
private:
	VkInstance mInstance;
};

