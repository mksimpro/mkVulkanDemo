#include "App.h"
#include <limits>

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

    // Query instance layers
    if (result == VK_SUCCESS) result = queryInstanceLayerProperties();
    // Query physical devices
    if (result == VK_SUCCESS) result = queryPhysicalDevices();
    // Query physical device layers
    if (result == VK_SUCCESS) result = queryPhysicalDeviceLayerProperties();
    // Query properties of physical devices
    if (result == VK_SUCCESS) result = queryPhysicalDeviceProperties();
    // Query features of physical devices
    if (result == VK_SUCCESS) result = queryPhysicalDeviceFeatures();
    // Query memory properties of physical devices
    if (result == VK_SUCCESS) result = queryPhysicalDeviceMemoryProperties();

    // Query queue familty properties of physical devices
    if (result == VK_SUCCESS) queryPhysicalDeviceQueueFamilyProperties();

    // Initialize logical device
    if (result == VK_SUCCESS)
    {
        // NVIDIA GeForce GTX 1060
        size_t physicalDeviceIndex{ 0u };

        // Find queueFamilyIndex for input queueFlags

        // queueFlags - combination of VkQueueFlagBits, where each flag means that queues in this family that supports or not the following :
        // VK_QUEUE_GRAPHICS_BIT                    // graphics operations : drawing points, lines, triangles
        // VK_QUEUE_COMPUTE_BIT                     // computer operations : e.g.dispatching compute shaders
        // VK_QUEUE_TRANSFER_BIT                    // transfer operations : e.g.copying bufferand image contents
        // VK_QUEUE_SPARSE_BINDING_BIT              // memory binding operations used to update sparse resources

        VkQueueFlags queueFlags{ VK_QUEUE_GRAPHICS_BIT };
        uint32_t queueFamilyIndex{ 0u };
        if (!getQueueFamilyIndex(physicalDeviceIndex, queueFlags, queueFamilyIndex))
            return VK_ERROR_UNKNOWN;

        // Create logical device
        result = createLogicalDevice(physicalDeviceIndex, queueFamilyIndex);
    }

    return result;
}

VkResult App::deinit()
{
    VkResult result = VK_SUCCESS;

    result = vkDeviceWaitIdle(mDevice);
    if (result == VK_SUCCESS)
        vkDestroyDevice(mDevice, nullptr);

    vkDestroyInstance(mInstance, nullptr);

    return result;
}

VkResult App::queryInstanceLayerProperties()
{
    // Query instance layers

    VkResult result = VK_SUCCESS;

    uint32_t propertyCount{ 0u };

    result = vkEnumerateInstanceLayerProperties(    // discover available layers to instance on system
        &propertyCount,                             // uint32_t * pPropertyCount,               // output - get number of layer properties
        nullptr                                     // VkLayerProperties * pProperties);        // nullptr
    );

    if (result == VK_SUCCESS)
    {
        mInstanceLayerProperties.resize(propertyCount);

        result = vkEnumerateInstanceLayerProperties( // discover available layers to instance on system
            &propertyCount,                         // uint32_t * pPropertyCount,               // input - provide number of layer properties
            mInstanceLayerProperties.data()         // VkLayerProperties * pProperties);        // array of structures to be filled with info about registered layers
        );
    }

    return result;
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

VkResult App::queryPhysicalDeviceLayerProperties()
{
    // Query device layer properties

    VkResult result = VK_SUCCESS;

    const size_t len{ mPhysicalDevices.size() };
    mPhysicalDeviceLayerProperties.resize(len);

    for (size_t i{ 0u }; i < len; ++i)
    {
        uint32_t propertyCount{ 0u };

        result = vkEnumerateDeviceLayerProperties(  // discover available layers to device on system
            mPhysicalDevices[i],                    // physical device to query
            &propertyCount,                         // uint32_t * pPropertyCount,                   // output - get number of layer properties
            nullptr                                 // VkLayerProperties * pProperties);            // nullptr
        );

        if (result == VK_SUCCESS)
        {
            mPhysicalDeviceLayerProperties[i].resize(propertyCount);

            result = vkEnumerateDeviceLayerProperties( // discover available layers to device on system
                mPhysicalDevices[i],                // physical device to query
                &propertyCount,                     // uint32_t * pPropertyCount,                   // output - get number of layer properties
                mPhysicalDeviceLayerProperties[i].data() // VkLayerProperties * pProperties);       // array of structures to be filled with info about registered layers
            );

            if (result != VK_SUCCESS)
                break;
        }
        else
            break;
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

VkResult App::queryPhysicalDeviceMemoryProperties()
{
    // Query memory properties of physical devices

    // typedef struct VkPhysicalDeviceMemoryProperties {        // properties of device heaps and supported memory types
    //    uint32_t memoryTypeCount;                             // number of memory types
    //    VkMemoryType memoryTypes[VK_MAX_MEMORY_TYPES];        // memoryTypeCount number of structures
    //    uint32_t memoryHeapCount;                             // number of heaps
    //    VkMemoryHeap memoryHeaps[VK_MAX_MEMORY_HEAPS];        // memoryHeapCount number of structures
    //} VkPhysicalDeviceMemoryProperties;

    VkResult result = VK_SUCCESS;

    const size_t len{ mPhysicalDevices.size() };
    mPhysicalDeviceMemoryProperties.resize(len);

    for (size_t i{ 0u }; i < len; ++i)
    {
        vkGetPhysicalDeviceMemoryProperties(        // fill structures describing all memory properties of physical device
            mPhysicalDevices[i],                    // VkPhysicalDevice physicalDevice,             // handle to physical device
            &mPhysicalDeviceMemoryProperties[i]     // VkPhysicalDeviceMemoryProperties* pMemoryProperties); // structure to be filled with details on physical device memory properties
        );
    }

    return result;
}

void App::queryPhysicalDeviceQueueFamilyProperties()
{
    // Query queue familty properties of physical devices

    // typedef struct VkQueueFamilyProperties {                 // identical queues with same caps and able to run in parallel
    //    VkQueueFlags queueFlags;                              // overall caps of queue
    //    uint32_t queueCount;                                  // number of queues in family (1 or more)
    //    uint32_t timestampValidBits;                          // number of valid bits when timestamps taken from queue
    //    VkExtent3D minImageTransferGranularity;               // units to support image transfers (may not support)
    //} VkQueueFamilyProperties;

    const size_t len{ mPhysicalDevices.size() };
    mQueueFamilyProperties.resize(len);
    uint32_t queueFamilyPropertyCount{ 0u };

    for (size_t i{ 0u }; i < len; ++i)
    {
        vkGetPhysicalDeviceQueueFamilyProperties(   // discover number of queue families supported by physical device
            mPhysicalDevices[i],                    // VkPhysicalDevice physicalDevice,             // handle to physical device
            &queueFamilyPropertyCount,              // uint32_t * pQueueFamilyPropertyCount,        // output - get number of queue families
            nullptr                                 // VkQueueFamilyProperties * pQueueFamilyProperties // nullptr
        );
        mQueueFamilyProperties[i].resize(queueFamilyPropertyCount);

        vkGetPhysicalDeviceQueueFamilyProperties(   // get properties of device queue families
            mPhysicalDevices[i],                    // VkPhysicalDevice physicalDevice,             // handle to physical device
            &queueFamilyPropertyCount,              // uint32_t * pQueueFamilyPropertyCount,        // input - provide number of queue families
            mQueueFamilyProperties[i].data()        // VkQueueFamilyProperties * pQueueFamilyProperties // array of structures to fill
        );
    }
}

bool App::getQueueFamilyIndex(size_t physicalDeviceIndex, VkQueueFlags queueFlags, uint32_t& queueFamilyIndex)
{
    const auto& queueFamilyProperties = mQueueFamilyProperties[physicalDeviceIndex];
    queueFamilyIndex = std::numeric_limits< uint32_t>::max();

    for (uint32_t i{ 0u }; i < queueFamilyProperties.size(); ++i)
        if (queueFamilyProperties[i].queueFlags & queueFlags)
        {
            queueFamilyIndex = i;
            break;
        }

    return queueFamilyIndex == std::numeric_limits< uint32_t>::max() ? false : true;
}

VkResult App::createLogicalDevice(size_t physicalDeviceIndex, uint32_t queueFamilyIndex)
{
    // Initialize logical device
    // 2) create device queue create info - VkDeviceQueueCreateInfo (queueFamilyIndex)
    // 3) create device create info - VkDeviceCreateInfo (&VkDeviceQueueCreateInfo)
    // 4) create device instance vkCreateDevice (&VkDeviceCreateInfo, &mDevice)
    // 5) destroy VkDeviceQueueCreateInfo and VkDeviceCreateInfo

    VkResult result = VK_SUCCESS;

    const VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, // VkStructureType sType;                   // type of create device info structure
        nullptr,                                // const void* pNext;                           // ptr to provide some version extensions, can be set to nullptr
        0,                                      // VkDeviceCreateFlags flags;                   // 0 (no bits defined in current version of Vulkan)
        queueFamilyIndex,                       // uint32_t queueFamilyIndex;                   // index to queues matching queueFlags
        1,                                      // uint32_t queueCount;                         // input - number of queues to create in the family queueFamilyIndex
        nullptr                                 // const float* pQueuePriorities;               // nullptr or ptr to array of floats representing priorities of queues
    };

    const VkDeviceCreateInfo deviceCreateInfo = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,   // VkStructureType sType;                       // type of create device info structure
        nullptr,                                // const void* pNext;                           // ptr to provide some version extensions, can be set to nullptr
        0,                                      // VkDeviceCreateFlags flags;                   // 0 (no bits defined in current version of Vulkan)
        1,                                      // uint32_t queueCreateInfoCount;               // number of structures in pQueueCreateInfos array
        &deviceQueueCreateInfo,                 // const VkDeviceQueueCreateInfo* pQueueCreateInfos; // ptr to array of structures with queues specs
        0,                                      // uint32_t enabledLayerCount;                  // let enable layers and extensions (can be 0)
        nullptr,                                // const char* const* ppEnabledLayerNames;		// let enable layers and extensions (can be nullptr)
        0,                                      // uint32_t enabledExtensionCount;              // let enable layers and extensions (can be 0)
        nullptr,                                // const char* const* ppEnabledExtensionNames;  // let enable layers and extensions (can be nullptr)
        &mPhysicalDeviceFeatures[physicalDeviceIndex] // const VkPhysicalDeviceFeatures* pEnabledFeatures; // ptr to structure with optional features (can be nullptr)
    };

    result = vkCreateDevice(                    // create logical device
        mPhysicalDevices[physicalDeviceIndex],  // VkPhysicalDevice physicalDevice,             // handle to physical device
        &deviceCreateInfo,                      // const VkDeviceCreateInfo * pCreateInfo,      // parameters describing logical device
        nullptr,                                // const VkAllocationCallbacks * pAllocator,    // custom memory allocator
        &mDevice                                // VkDevice * pDevice);                         // handle to logical device
    );

    return result;
}
