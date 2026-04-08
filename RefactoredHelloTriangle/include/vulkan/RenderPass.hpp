#pragma once

#include <vulkan/vulkan_core.h>

class RenderPass {
public:
    RenderPass() = default;
    ~RenderPass();

    RenderPass(const RenderPass&) = delete;
    RenderPass& operator=(const RenderPass&) = delete;

    VkRenderPass get() const { return renderPass_; }

    void create(VkDevice device, VkFormat imageFormat);

private:
    VkRenderPass renderPass_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
};
