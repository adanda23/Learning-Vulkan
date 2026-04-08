#pragma once

#include <vulkan/vulkan_core.h>

class PhysicalDevice;

class LogicalDevice {
public:
    LogicalDevice() = default;
    ~LogicalDevice();

    LogicalDevice(const LogicalDevice&) = delete;
    LogicalDevice& operator=(const LogicalDevice&) = delete;
    LogicalDevice(LogicalDevice&&) = default;
    LogicalDevice& operator=(LogicalDevice&&) = default;

    VkDevice get() const { return device_; }
    VkQueue getGraphicsQueue() const { return graphicsQueue_; }
    VkQueue getPresentQueue() const { return presentQueue_; }

    void create(const PhysicalDevice& physicalDevice, VkSurfaceKHR surface);

private:
    VkDevice device_ = VK_NULL_HANDLE;
    VkQueue graphicsQueue_;
    VkQueue presentQueue_;
};
