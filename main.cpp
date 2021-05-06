#include "App.h"
#include <vulkan/vulkan.h>

int main()
{
	App app;

	if (app.init() != VK_SUCCESS)
		return -1;

	if (app.deinit() != VK_SUCCESS)
		return -1;

	return 0;
}