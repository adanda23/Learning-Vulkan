#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <set>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 600;

constexpr std::array<const char*, 1> VALIDATION_LAYERS = {{
    "VK_LAYER_KHRONOS_validation"
}};

constexpr std::array<const char*, 1> DEVICE_EXTENSIONS = {{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
}};

constexpr bool ENABLE_VALIDATION_LAYERS = true;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
