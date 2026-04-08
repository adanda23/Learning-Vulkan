#pragma once

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class VulkanInstance {
public:
    VulkanInstance() = default;
    ~VulkanInstance();

    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;
    VulkanInstance(VulkanInstance&&) = default;
    VulkanInstance& operator=(VulkanInstance&&) = default;

    VkInstance get() const { return instance_; }

    void create(bool enableValidationLayers, GLFWwindow* window);

private:
    VkInstance instance_ = VK_NULL_HANDLE;

    bool checkValidationLayerSupport() const;
    std::vector<const char*> getRequiredExtensions() const;
    void createInstance(bool enableValidationLayers, GLFWwindow* window);
};
