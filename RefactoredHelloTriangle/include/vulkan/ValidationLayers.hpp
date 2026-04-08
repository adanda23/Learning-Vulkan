#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

class ValidationLayers {
public:
    ValidationLayers() = default;
    ~ValidationLayers() = default;

    ValidationLayers(const ValidationLayers&) = delete;
    ValidationLayers& operator=(const ValidationLayers&) = delete;

    bool checkSupport() const;
    const std::vector<const char*>& getLayers() const { return layers_; }

private:
    std::vector<const char*> layers_ = {
        "VK_LAYER_KHRONOS_validation"
    };
};
