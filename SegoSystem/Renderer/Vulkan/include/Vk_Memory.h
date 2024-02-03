#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vulkan::memory
{
    void transitionImageLayout(VkCommandBuffer cmdBuffer,VkImage image,VkImageLayout oldLayout,
    VkImageLayout newLayout,uint32_t mipBase,uint32_t mipCount,uint32_t numBase,uint32_t numArray,
    VkImageAspectFlagBits aspectFlags= VK_IMAGE_ASPECT_COLOR_BIT /*= VK_IMAGE_ASPECT_COLOR_BIT*/);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

};