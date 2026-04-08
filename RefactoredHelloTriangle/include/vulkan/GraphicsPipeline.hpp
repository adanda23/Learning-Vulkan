#pragma once

#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

class GraphicsPipeline {
public:
    GraphicsPipeline() = default;
    ~GraphicsPipeline();

    GraphicsPipeline(const GraphicsPipeline&) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

    VkPipeline get() const { return pipeline_; }
    VkPipelineLayout getLayout() const { return pipelineLayout_; }

    void create(VkDevice device, VkRenderPass renderPass, VkExtent2D swapChainExtent,
                const std::string& vertShaderPath, const std::string& fragShaderPath);

private:
    VkDevice device_ = VK_NULL_HANDLE;
    VkPipeline pipeline_ = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout_ = VK_NULL_HANDLE;

    std::vector<uint32_t> readFile(const std::string& filename) const;
    VkShaderModule createShaderModule(VkDevice device, const std::vector<uint32_t>& code) const;
};
