#include "vk_device.h"

static const char *validation_layers[] = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
static const bool enable_validation_layers = false;
#else
static const bool enable_validation_layers = true;
#endif

static void create_instance(vk_device_t *device)
{
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Vulkan App";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    u32 glfw_extension_count = 0;
    const char **glfw_extensions = glfwGetRequiredInstanceExtensions(
        &glfw_extension_count
    );

    create_info.enabledExtensionCount = glfw_extension_count;
    create_info.ppEnabledExtensionNames = glfw_extensions;

    if (enable_validation_layers) {
        create_info.enabledLayerCount = ARR_LEN(validation_layers);
        create_info.ppEnabledLayerNames = validation_layers;
    } else {
        create_info.enabledLayerCount = 0;
    }

    VkResult res = vkCreateInstance(&create_info, NULL, &device->instance);
    if (res != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance\n");
        vk_device_destroy(device);
    }
}

vk_device_t *vk_device_create(GLFWwindow *window)
{
    UNUSED(window);

    vk_device_t *device = malloc(sizeof(vk_device_t));
    if (device == NULL) {
        return NULL;
    }

    create_instance(device);

    return device;
}

void vk_device_destroy(vk_device_t *device)
{
    if (device == NULL) { return; }

    vkDestroyDevice(device->device, NULL);
    vkDestroySurfaceKHR(device->instance, device->surface, NULL);
    vkDestroyDebugUtilsMessengerEXT(device->instance, device->debug, NULL);
    vkDestroyInstance(device->instance, NULL);

    free(device);
    device = NULL;
}