#include "App.h"

VkResult App::init()
{
    // Initialize Vulkan instance
    // 1) create user app info - VkApplicationInfo
    // 2) create instace create info - VkInstanceCreateInfo (&VkApplicationInfo)
    // 3) create Vulkan instance - vkCreateInstance (&VkInstanceCreateInfo, &mInstance)
    // 4) destroy VkApplicationInfo and VkInstanceCreateInfo

    VkResult result = VK_SUCCESS;

    // applicationInfo and instanceCreateInfo will be consumed (internal copy)
    {
        const VkApplicationInfo applicationInfo = {
            VK_STRUCTURE_TYPE_APPLICATION_INFO,     // VkStructureType sType;                       // type of applicaiton info structure
            nullptr,                                // const void* pNext;                           // ptr to provide some version extensions, can be set to nullptr
            "mkVulkanDemo",                         // const char* pApplicationName;                // null-terminated string for user app name
            1,                                      // uint32_t applicationVersion;                 // user app version - info for tools and drivers how to treat the app
            "Engine",                               // const char* pEngineName;                     // name of engine or middleware that user app is based on
            1,                                      // uint32_t engineVersion;                      // version (..)
            VK_MAKE_VERSION(1,2,0)                  // uint32_t apiVersion;                         // Vulkan API version (should be set to minimum to run user app)
        };

        const VkInstanceCreateInfo instanceCreateInfo = {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, // VkStructureType sType;                       // type of create info structure
            nullptr,                                // const void* pNext;                           // allows to pass a linked list of structures
            0,                                      // VkInstanceCreateFlags flags;	                // reserved for future use (set to 0)
            &applicationInfo,                       // const VkApplicationInfo* pApplicationInfo;   // optional for another struct describing user app (should be filled)
            0,                                      // uint32_t enabledLayerCount;	                // number of instance layer to enable (possible 0)
            nullptr,                                // const char* const* ppEnabledLayerNames;      // names for enabled layer instances (possible nullptr)
            0,                                      // uint32_t enabledExtensionCount;              // number of extensions to enable (possible 0)
            nullptr                                 // const char* const* ppEnabledExtensionNames;  // names of enabled extensions (possible nullptr)
        };

        result = vkCreateInstance(	                // creates VkInstance
            &instanceCreateInfo,                    // const VkInstanceCreateInfo* pCreateInfo,     // parameters describing VkInstance
            nullptr,                                // const VkAllocationCallbacks* pAllocator,     // host memory for user app and Vulkan systems (nullptr to use Vulkan internal allocator)
            &mInstance                              // VkInstance* pInstance);                      // Vulkan handle (64-bits) for calling other functions
        );
    }

    return result;
}

void App::deinit()
{
    vkDestroyInstance(mInstance, nullptr);
}
