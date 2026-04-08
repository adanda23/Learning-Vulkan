#pragma once

#include <vulkan/vulkan_core.h>

class CommandBuffer {
public:
    CommandBuffer() = default;
    ~CommandBuffer() = default;

    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer& operator=(const CommandBuffer&) = delete;

    VkCommandBuffer get() const { return commandBuffer_; }
    VkCommandBuffer& get() { return commandBuffer_; }

    void allocate(VkDevice device, VkCommandPool commandPool);
    void begin();
    void end();
    void reset();

    void record(VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D extent,
                VkPipeline graphicsPipeline, VkExtent2D swapChainExtent);

private:
    VkCommandBuffer commandBuffer_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
};
