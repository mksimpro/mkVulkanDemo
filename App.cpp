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

    // Query physical devices
    if (result == VK_SUCCESS) result = queryPhysicalDevices();
    // Query properties of physical devices
    if (result == VK_SUCCESS) result = queryPhysicalDeviceProperties();
    // Query features of physical devices
    if (result == VK_SUCCESS) result = queryPhysicalDeviceFeatures();

    return result;
}

void App::deinit()
{
    vkDestroyInstance(mInstance, nullptr);
}

VkResult App::queryPhysicalDevices()
{
    // Query physical devices
    // 1) discover all supported devices in the system
    // 2) get handles to all physical devices in the system

    VkResult result = VK_SUCCESS;

    if (result == VK_SUCCESS)
    {
        uint32_t physicalDeviceCount{ 0u };

        result = vkEnumeratePhysicalDevices(        // discover all supported devices in the system
            mInstance,                              // VkInstance instance,                         // Vulkan instance created in vkCreateInstance
            &physicalDeviceCount,                   // uint32_t * pPhysicalDeviceCount,             // output - get number of physical devices
            nullptr                                 // VkPhysicalDevice * pPhysicalDevices);        // nullptr
        );

        if (result == VK_SUCCESS)
        {
            mPhysicalDevices.resize(physicalDeviceCount);

            result = vkEnumeratePhysicalDevices(    // get handles to all physical devices in the system
                mInstance,                          // VkInstance instance,                         // Vulkan instance created in vkCreateInstance
                &physicalDeviceCount,               // uint32_t * pPhysicalDeviceCount,             // input - provide number of discovered physical devices in the system
                &mPhysicalDevices[0]                // VkPhysicalDevice * pPhysicalDevices);        // array of handles to physical devices
            );
        }
    }

    return result;
}

VkResult App::queryPhysicalDeviceProperties()
{
    // Query properties of physical devices

    // typedef struct VkPhysicalDeviceProperties {              // details on physical device
    //    uint32_t apiVersion;                                  // highest version of Vulkan supported by device
    //    uint32_t driverVersion;                               // driver version used to control device (vendor specific)
    //    uint32_t vendorID;                                    // identify vendor
    //    uint32_t deviceID;                                    // identify device
    //    VkPhysicalDeviceType deviceType;                      // supported physical device types
    //    char deviceName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE];    // human readable name of device
    //    uint8_t pipelineCacheUUID[VK_UUID_SIZE];              // used for pipeline caching
    //    VkPhysicalDeviceLimits limits;                        // min and max limits for physical device
    //    VkPhysicalDeviceSparseProperties sparseProperties;    // properties related to sparse textures
    //} VkPhysicalDeviceProperties;

    VkResult result = VK_SUCCESS;

    const size_t len{ mPhysicalDevices.size() };
    mPhysicalDeviceProperties.resize(len);

    for (size_t i{ 0u }; i < len; ++i)
    {
        vkGetPhysicalDeviceProperties(              // fill structures describing all properties of physical device
            mPhysicalDevices[i],                    // VkPhysicalDevice physicalDevice,             // handle to physical device
            &mPhysicalDeviceProperties[i]           // VkPhysicalDeviceProperties * pProperties);   // structure to be filled with details on physical device properties
        );
    }

    return result;
}

VkResult App::queryPhysicalDeviceFeatures()
{
    // Query features of physical devices

    // typedef struct VkPhysicalDeviceFeatures {                // details on features of physical device
    //    VkBool32    robustBufferAccess;                       // bool field marked if the feature is supported
    //    ..
    //} VkPhysicalDeviceFeatures;

    VkResult result = VK_SUCCESS;

    const size_t len{ mPhysicalDevices.size() };
    mPhysicalDeviceFeatures.resize(len);

    for (size_t i{ 0u }; i < len; ++i)
    {
        vkGetPhysicalDeviceFeatures(                // fill structures describing all features of physical device
            mPhysicalDevices[i],                    // VkPhysicalDevice physicalDevice,             // handle to physical device
            &mPhysicalDeviceFeatures[i]             // VkPhysicalDeviceFeatures* pFeatures);        // structure to be filled with details on physical device features
        );
    }

    return result;
}
