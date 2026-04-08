#include "vulkan/CommandPool.hpp"

#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

CommandPool::~CommandPool() {
    if (commandPool_ != VK_NULL_HANDLE && device_ != VK_NULL_HANDLE) {
        vkDestroyCommandPool(device_, commandPool_, nullptr);
    }
}

void CommandPool::create(VkDevice device, uint32_t queueFamilyIndex) {
    device_ = device;

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
    std::cout << "Command pool created successfully!" << std::endl;
}
