#pragma once

#include <vulkan/vulkan_core.h>

class SyncObjects {
public:
    SyncObjects() = default;
    ~SyncObjects();

    SyncObjects(const SyncObjects&) = delete;
    SyncObjects& operator=(const SyncObjects&) = delete;

    VkSemaphore getImageAvailable() const { return imageAvailableSemaphore_; }
    VkSemaphore getRenderFinished() const { return renderFinishedSemaphore_; }
    VkFence& getInFlight() { return inFlightFence_; }

    void create(VkDevice device);

private:
    VkDevice device_ = VK_NULL_HANDLE;
    VkSemaphore imageAvailableSemaphore_;
    VkSemaphore renderFinishedSemaphore_;
    VkFence inFlightFence_;
};
