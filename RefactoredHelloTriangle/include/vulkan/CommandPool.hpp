#pragma once

#include <vulkan/vulkan_core.h>

class CommandPool {
public:
    CommandPool() = default;
    ~CommandPool();

    CommandPool(const CommandPool&) = delete;
    CommandPool& operator=(const CommandPool&) = delete;

    VkCommandPool get() const { return commandPool_; }

    void create(VkDevice device, uint32_t queueFamilyIndex);

private:
    VkCommandPool commandPool_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
};
