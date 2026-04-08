#pragma once

#include <vulkan/vulkan_core.h>
#include "Common.hpp"

class PhysicalDevice {
public:
    PhysicalDevice() = default;
    ~PhysicalDevice() = default;

    PhysicalDevice(const PhysicalDevice&) = delete;
    PhysicalDevice& operator=(const PhysicalDevice&) = delete;

    VkPhysicalDevice get() const { return physicalDevice_; }

    void pick(VkInstance instance, VkSurfaceKHR surface);
    bool isSuitable() const { return physicalDevice_ != VK_NULL_HANDLE; }

    QueueFamilyIndices getQueueFamilies(VkSurfaceKHR surface) const;
    SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR surface) const;

private:
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) const;
    bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const;
};
