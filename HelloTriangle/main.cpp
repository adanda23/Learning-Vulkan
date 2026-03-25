// The commented out lines are for windows, I am developing on linux so I have them disabled

// #define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #define GLFW_EXPOSE_NATIVE_WIN32
// #include <GLFW/glfw3native.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <optional>
#include <ctime>

// Use constants for window
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// Creating a validation layer for our instances
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

// Enable validation layers, disable for release
const bool enableValidationLayers = true;

// Holds indices of queue families we want to use
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

class HelloTriangleApplication {
public:
    void run() {
        std::clock_t start = std::clock();
        initWindow();
        showExtensions();
        initVulkan();
        std::clock_t end = std::clock();
        printf("Initialization took %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window = nullptr;
    VkInstance instance;
    VkDevice device;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue;
    //an abstract type of surface to present rendered images to
    VkSurfaceKHR surface;

    void initWindow() {
        glfwInit();
        // Tell to not use OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // Disable window resizing
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
    }

    void showExtensions() {
        //Use vkEnumerateInstanceExtensionProperties to find possible extensions
        //Extensions are optional features that can be added
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::cout << "Available Vulkan extensions:" << std::endl;
        for (const auto& extension : extensions) {
            std::cout << "\t" << extension.extensionName << std::endl;
        }
    }

    void createSurface() {
        // Create a surface for rendering, this is platform specific but GLFW abstracts it away
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        } else {
            std::cout << "Vulkan surface created successfully!" << std::endl;
        }
    }

    void pickPhysicalDevice() {
        //Picks a gpu to use
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        // go through each device and check if its usable
        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    bool isDeviceSuitable(VkPhysicalDevice device) {
        //Check for properties (name, type, supported vulkan version, etc)
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        //name of gpu
        std::cout << "Available GPU: " << deviceProperties.deviceName << std::endl;

        //Check for optional features *texture compression, 64 bit floats, multiviewport rendering
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        QueueFamilyIndices indices = findQueueFamilies(device);
        // this example returns true if gpu has geometry shader support and is discrete
        // You can also rank devices based on which have the most features
        // return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
        return indices.isComplete();
    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        // Everything in vulkan is executed on queues, there ae different types of queues for different types of operations (graphics, compute, transfer)
        // this will find graphics queues
        QueueFamilyIndices indices;
        uint32_t queueFamilyCount = 0;
        // Get the number of queue families supported by the device
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        // Retrieve the properties of each queue family
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            // Check if the queue family supports graphics operations
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    void createInstance() {
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        //Pass in optional info that driver might use for optimizatoin
        VkApplicationInfo appInfo{};
        //sType usutally specifies the structure we are using
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        //This step is not optional, we have to define which extensions Vulkan will use. GLFW 
        //can also tell us which it need specifically
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        // Get the extensions required by GLFW for Vulkan to interface with the window system
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        // Enable validation layers 
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        //Finally create the instance
        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create instance!");
        } else {
            std::cout << "Vulkan instance created successfully!" << std::endl;
        }
    }

    void createLogicalDevice() {
        // Logical device allows us to interface with the physical device and use its features
        // We have to specify which queues we want to use from the physical device, and also which features we want to use
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        // Specify the queues we want to create for the logical device
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        //Required step, sets the proiority of the queue frmo 0 and 1
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        
        // will pass device queue create info into logical device
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        VkPhysicalDeviceFeatures deviceFeatures{};
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = 0;

        // Deprecated in modern vulkan, only need it at the instance level
        //
        // if (enableValidationLayers) {
        //     createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        //     createInfo.ppEnabledLayerNames = validationLayers.data();
        // } else {
        //     createInfo.enabledLayerCount = 0;
        // }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        } else {
            std::cout << "Vulkan logical device created successfully!" << std::endl;
        }

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    }
    

    void mainLoop() {
        while (window && !glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }

    }


    void cleanup() {
        // destroy vulkan instance and window and logical device
        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);

        glfwTerminate();
    }

    // Checks for layer properties, similar to how we checked extensions
    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // loop through our validation layers and see which layers match up with the layers we want
        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}