#include "vk_device.h"

const char *validation_layers[] = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif

const char *device_extensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static void create_command_pool(vk_device_t *device)
{
    queue_family_indices_t indices = device->indices;

    VkCommandPoolCreateInfo pool_info = {0};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = indices.graphics_family;
    pool_info.flags = 0;

    VkResult res = vkCreateCommandPool(
        device->device,
        &pool_info,
        NULL,
        &device->command_pool
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create command pool!");
    }
}

static queue_family_indices_t find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    queue_family_indices_t indices = {0};

    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties *queue_families = malloc(
        queue_family_count * sizeof(VkQueueFamilyProperties)
    );

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (u32 i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = i;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);

        if (present_support) {
            indices.present_family = i;
        }

        if (indices.graphics_family >= 0 && indices.present_family >= 0) {
            break;
        }
    }

    free(queue_families);
    return indices;
}

static void create_logical_device(vk_device_t *device)
{
    device->indices = find_queue_families(device->physical_device, device->surface);

    VkDeviceQueueCreateInfo queue_create_info = {0};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = device->indices.graphics_family;
    queue_create_info.queueCount = 1;
    f32 queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features = {0};

    VkDeviceCreateInfo create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.queueCreateInfoCount = 1;
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = ARR_LEN(device_extensions);
    create_info.ppEnabledExtensionNames = device_extensions;

    if (enable_validation_layers) {
        create_info.enabledLayerCount = ARR_LEN(validation_layers);
        create_info.ppEnabledLayerNames = validation_layers;
    } else {
        create_info.enabledLayerCount = 0;
    }

    VkResult res = vkCreateDevice(
        device->physical_device,
        &create_info,
        NULL,
        &device->device
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create logical device!");
    }

    vkGetDeviceQueue(
        device->device,
        device->indices.graphics_family,
        0,
        &device->graphics_queue
    );

    vkGetDeviceQueue(
        device->device,
        device->indices.present_family,
        0,
        &device->present_queue
    );
}

static u32 rate_device(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

    u32 score = 0;

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    score += properties.limits.maxImageDimension2D;

    if (!features.geometryShader) {
        return 0;
    }

    return score;
}

static void pick_physical_device(vk_device_t *device)
{
    u32 device_count = 0;
    vkEnumeratePhysicalDevices(device->instance, &device_count, NULL);

    if (device_count == 0) {
        LOG_ERROR("Failed to find GPUs with Vulkan support!");
    }

    VkPhysicalDevice *devices = malloc(device_count * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(device->instance, &device_count, devices);

    u32 best_score = 0;
    for (u32 i = 0; i < device_count; i++) {
        u32 score = rate_device(devices[i]);
        if (score > best_score) {
            device->physical_device = devices[i];
            best_score = score;
        }
    }

    if (device->physical_device == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to find a suitable GPU!");
    }

    free(devices);
}

static void create_surface(vk_device_t *device)
{
    VkResult res = glfwCreateWindowSurface(
        device->instance,
        device->window->handle,
        NULL,
        &device->surface
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create window surface!");
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data
) {
    UNUSED(user_data);
    UNUSED(message_type);

    switch(message_severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LOG_WARN(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LOG_ERROR(callback_data->pMessage);
            break;
        default:
            break;
    }

    return VK_FALSE;
}

VkResult create_debug_utils_messenger_ext(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *create_info,
    const VkAllocationCallbacks *allocator,
    VkDebugUtilsMessengerEXT *debug_messenger
) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL) {
        return func(instance, create_info, allocator, debug_messenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void destroy_debug_utils_messenger_ext(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debug_messenger,
    const VkAllocationCallbacks *allocator
) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != NULL) {
        func(instance, debug_messenger, allocator);
    }
}

static void populate_debug_messenger_create_info(
    VkDebugUtilsMessengerCreateInfoEXT *create_info
) {
    create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info->pfnUserCallback = debug_callback;
    create_info->pUserData = NULL;
    create_info->pNext = NULL;
    create_info->flags = 0;
}

static void setup_debug_messenger(vk_device_t *device)
{
    if (!enable_validation_layers) {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT create_info;
    populate_debug_messenger_create_info(&create_info);

    if (create_debug_utils_messenger_ext(
        device->instance,
        &create_info,
        NULL,
        &device->debug
    ) != VK_SUCCESS) {
        LOG_ERROR("Failed to set up debug messenger!");
    }
}

static bool check_validation_layer_support()
{
    u32 layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties *available_layers = malloc(
        layer_count * sizeof(VkLayerProperties)
    );

    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

    for (u32 i = 0; i < layer_count; i++) {
        if (strcmp(
            "VK_LAYER_KHRONOS_validation",
            available_layers[i].layerName
        ) == 0) {
            free(available_layers);
            return true;
        }
    }

    free(available_layers);
    return false;
}

static void create_instance(vk_device_t *device)
{
    if (enable_validation_layers && !check_validation_layer_support()) {
        LOG_ERROR("Validation layers requested, but not available!");
    }

    VkApplicationInfo app_info = {0};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Vulkan App";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {0};
    if (enable_validation_layers) {
        populate_debug_messenger_create_info(&debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debug_create_info;

        create_info.enabledLayerCount = ARR_LEN(validation_layers);
        create_info.ppEnabledLayerNames = validation_layers;
    } else {
        create_info.enabledLayerCount = 0;
        create_info.pNext = NULL;
    }

    u32 glfw_extension_count = 0;
    const char **glfw_extensions = glfwGetRequiredInstanceExtensions(
        &glfw_extension_count
    );

    u32 extension_count = glfw_extension_count;
    const char **extensions = malloc(
        (extension_count + 1) * sizeof(const char *)
    );

    memcpy(
        extensions,
        glfw_extensions,
        glfw_extension_count * sizeof(const char *)
    );

    if (enable_validation_layers) {
        extensions[extension_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        extension_count++;
    }

    create_info.enabledExtensionCount = extension_count;
    create_info.ppEnabledExtensionNames = extensions;

    VkResult res = vkCreateInstance(&create_info, NULL, &device->instance);
    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create instance!");
    }
}

vk_device_t *vk_device_create(window_t *window)
{
    if (!glfwVulkanSupported()) {
        LOG_ERROR("GLFW Vulkan not supported!");
        return NULL;
    }

    vk_device_t *device = malloc(sizeof(vk_device_t));
    if (!device) {
        return NULL;
    }

    device->window = window;

    create_instance(device);
    setup_debug_messenger(device);
    create_surface(device);
    pick_physical_device(device);
    create_logical_device(device);
    create_command_pool(device);

    return device;
}

void vk_device_destroy(vk_device_t *device)
{
    if (!device) {
        return;
    }

    vkDestroyCommandPool(device->device, device->command_pool, NULL);
    vkDestroyDevice(device->device, NULL);
    vkDestroySurfaceKHR(device->instance, device->surface, NULL);

    if (enable_validation_layers) {
        destroy_debug_utils_messenger_ext(
            device->instance,
            device->debug,
            NULL
        );
    }

    vkDestroyInstance(device->instance, NULL);
    free(device);

    device = NULL;
}