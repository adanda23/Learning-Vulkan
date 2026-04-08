#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include "Common.hpp"

class SwapChain {
public:
    SwapChain() = default;
    ~SwapChain();

    SwapChain(const SwapChain&) = delete;
    SwapChain& operator=(const SwapChain&) = delete;

    VkSwapchainKHR get() const { return swapChain_; }
    VkFormat getImageFormat() const { return imageFormat_; }
    VkExtent2D getExtent() const { return extent_; }
    const std::vector<VkImage>& getImages() const { return images_; }

    void create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window);

private:
    VkSwapchainKHR swapChain_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
    VkFormat imageFormat_;
    VkExtent2D extent_;
    std::vector<VkImage> images_;

    SwapChainSupportDetails querySupport(VkPhysicalDevice device, VkSurfaceKHR surface) const;
    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available) const;
    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& available) const;
    VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) const;
};
