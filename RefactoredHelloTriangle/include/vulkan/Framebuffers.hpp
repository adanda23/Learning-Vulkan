#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

class Framebuffers {
public:
    Framebuffers() = default;
    ~Framebuffers();

    Framebuffers(const Framebuffers&) = delete;
    Framebuffers& operator=(const Framebuffers&) = delete;

    const std::vector<VkFramebuffer>& get() const { return framebuffers_; }

    void create(VkDevice device, VkRenderPass renderPass, const std::vector<VkImageView>& imageViews, VkExtent2D extent);

private:
    std::vector<VkFramebuffer> framebuffers_;
    VkDevice device_ = VK_NULL_HANDLE;
};
