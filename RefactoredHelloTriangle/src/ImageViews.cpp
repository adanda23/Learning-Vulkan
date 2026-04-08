#include "vulkan/ImageViews.hpp"

#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

ImageViews::~ImageViews() {
    if (device_ != VK_NULL_HANDLE) {
        for (auto imageView : imageViews_) {
            vkDestroyImageView(device_, imageView, nullptr);
        }
    }
}

void ImageViews::create(VkDevice device, const std::vector<VkImage>& images, VkFormat format) {
    device_ = device;
    imageViews_.resize(images.size());

    for (size_t i = 0; i < images.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &imageViews_[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
    std::cout << "Image views created successfully!" << std::endl;
}
