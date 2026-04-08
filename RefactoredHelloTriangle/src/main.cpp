#include <iostream>
#include <stdexcept>
#include <ctime>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/Common.hpp"
#include "vulkan/Instance.hpp"
#include "vulkan/Surface.hpp"
#include "vulkan/PhysicalDevice.hpp"
#include "vulkan/LogicalDevice.hpp"
#include "vulkan/SwapChain.hpp"
#include "vulkan/ImageViews.hpp"
#include "vulkan/RenderPass.hpp"
#include "vulkan/GraphicsPipeline.hpp"
#include "vulkan/Framebuffers.hpp"
#include "vulkan/CommandPool.hpp"
#include "vulkan/CommandBuffer.hpp"
#include "vulkan/SyncObjects.hpp"

class HelloTriangleApplication {
public:
    void run() {
        std::clock_t start = std::clock();
        initWindow();
        initVulkan();
        std::clock_t end = std::clock();
        printf("Initialization took %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window = nullptr;
    VulkanInstance instance;
    VulkanSurface surface;
    PhysicalDevice physicalDevice;
    LogicalDevice logicalDevice;
    SwapChain swapChain;
    ImageViews imageViews;
    RenderPass renderPass;
    GraphicsPipeline graphicsPipeline;
    Framebuffers framebuffers;
    CommandPool commandPool;
    CommandBuffer commandBuffer;
    SyncObjects syncObjects;

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {
        instance.create(ENABLE_VALIDATION_LAYERS, window);
        surface.create(instance.get(), window);
        physicalDevice.pick(instance.get(), surface.get());
        logicalDevice.create(physicalDevice, surface.get());
        swapChain.create(logicalDevice.get(), physicalDevice.get(), surface.get(), window);
        imageViews.create(logicalDevice.get(), swapChain.getImages(), swapChain.getImageFormat());
        renderPass.create(logicalDevice.get(), swapChain.getImageFormat());
        graphicsPipeline.create(logicalDevice.get(), renderPass.get(), swapChain.getExtent(),
                                "shaders/vert.spv", "shaders/frag.spv");
        framebuffers.create(logicalDevice.get(), renderPass.get(), imageViews.get(), swapChain.getExtent());
        
        auto queueFamilies = physicalDevice.getQueueFamilies(surface.get());
        commandPool.create(logicalDevice.get(), queueFamilies.graphicsFamily.value());
        commandBuffer.allocate(logicalDevice.get(), commandPool.get());
        syncObjects.create(logicalDevice.get());
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(logicalDevice.get());
    }

    void drawFrame() {
        vkWaitForFences(logicalDevice.get(), 1, &syncObjects.getInFlight(), VK_TRUE, UINT64_MAX);
        vkResetFences(logicalDevice.get(), 1, &syncObjects.getInFlight());

        uint32_t imageIndex;
        vkAcquireNextImageKHR(logicalDevice.get(), swapChain.get(), UINT64_MAX, syncObjects.getImageAvailable(), VK_NULL_HANDLE, &imageIndex);

        commandBuffer.reset();
        commandBuffer.begin();
        commandBuffer.record(renderPass.get(), framebuffers.get()[imageIndex], swapChain.getExtent(),
                            graphicsPipeline.get(), swapChain.getExtent());
        commandBuffer.end();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {syncObjects.getImageAvailable()};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.get();

        VkSemaphore signalSemaphores[] = {syncObjects.getRenderFinished()};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(logicalDevice.getGraphicsQueue(), 1, &submitInfo, syncObjects.getInFlight()) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain.get()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(logicalDevice.getPresentQueue(), &presentInfo);
    }

    void cleanup() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
