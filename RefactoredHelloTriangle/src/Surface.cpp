#include "vulkan/Surface.hpp"

#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

VulkanSurface::~VulkanSurface() {
    if (surface_ != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance_, surface_, nullptr);
    }
}

void VulkanSurface::create(VkInstance instance, GLFWwindow* window) {
    instance_ = instance;
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    std::cout << "Vulkan surface created successfully!" << std::endl;
}
