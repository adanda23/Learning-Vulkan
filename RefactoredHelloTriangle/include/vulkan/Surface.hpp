#pragma once

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

class VulkanSurface {
public:
    VulkanSurface() = default;
    ~VulkanSurface();

    VulkanSurface(const VulkanSurface&) = delete;
    VulkanSurface& operator=(const VulkanSurface&) = delete;

    VkSurfaceKHR get() const { return surface_; }

    void create(VkInstance instance, GLFWwindow* window);

private:
    VkInstance instance_ = VK_NULL_HANDLE;
    VkSurfaceKHR surface_ = VK_NULL_HANDLE;
};
