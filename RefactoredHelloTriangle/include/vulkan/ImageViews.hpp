#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

class ImageViews {
public:
    ImageViews() = default;
    ~ImageViews();

    ImageViews(const ImageViews&) = delete;
    ImageViews& operator=(const ImageViews&) = delete;

    const std::vector<VkImageView>& get() const { return imageViews_; }

    void create(VkDevice device, const std::vector<VkImage>& images, VkFormat format);

private:
    std::vector<VkImageView> imageViews_;
    VkDevice device_ = VK_NULL_HANDLE;
};
