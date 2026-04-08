#include "vulkan/ShaderModule.hpp"

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vulkan/vulkan_core.h>

ShaderModule::~ShaderModule() {
    if (shaderModule_ != VK_NULL_HANDLE && device_ != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device_, shaderModule_, nullptr);
    }
}

std::vector<uint32_t> ShaderModule::readFile(const std::string& filename) const {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    file.close();

    printf("Shader file size: %zu bytes\n", fileSize);

    return buffer;
}

void ShaderModule::create(VkDevice device, const std::string& filename) {
    device_ = device;

    auto code = readFile(filename);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size() * sizeof(uint32_t);
    createInfo.pCode = code.data();

    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    std::cout << "Shader module created successfully!" << std::endl;
}
