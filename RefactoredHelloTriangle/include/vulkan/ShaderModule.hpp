#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

class ShaderModule {
public:
    ShaderModule() = default;
    ~ShaderModule();

    ShaderModule(const ShaderModule&) = delete;
    ShaderModule& operator=(const ShaderModule&) = delete;

    VkShaderModule get() const { return shaderModule_; }

    void create(VkDevice device, const std::string& filename);

private:
    VkShaderModule shaderModule_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;

    std::vector<uint32_t> readFile(const std::string& filename) const;
};
